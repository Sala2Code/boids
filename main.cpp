#include "common/include.hpp"
#include "include/constants.hpp"
#include <functional>

#include "common/loadShader.hpp"

#include "include/display.hpp"
#include "include/boids.hpp"

int main()
{
	glfwInit();
    	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Simple", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); 
	gladLoadGL();
	glViewport(0, 0, width, height);

	GLuint shaderProgramBasic = LoadShaders("shader/basic.vert", "shader/basic.frag");

	// * VAO & VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// * Generate boids
	std::vector<Boids> listBoids;
	for(unsigned int i = 0; i<nbBoids+1; i++){
		listBoids.push_back(Boids());
	}

	float boidsVertices[(nbBoids+1)*2*3] = {0}; // 2 is (x;y) and 3 couples and +1 for the predator !
	GLuint vertexbufferBoids;
	glGenBuffers(1, &vertexbufferBoids);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBoids);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boidsVertices), boidsVertices, GL_STATIC_DRAW);

	float boidsColors[(nbBoids+1)*3*3] = {0};

	GLuint colorbufferBoids;
	glGenBuffers(1, &colorbufferBoids);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferBoids);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boidsColors), boidsColors, GL_STATIC_DRAW);

	GLfloat newTriangle[(nbBoids+1)*2*3];
	GLfloat newColors[(nbBoids+1)*3*3] = {0};

	for(unsigned short i=0; i<nbBoids; i++){
		glm::vec2 pos0 = listBoids[i].getPos(0);
		newTriangle[2*3*i] = pos0.x;
		newTriangle[2*3*i+1] = pos0.y;

		glm::vec2 pos1 = listBoids[i].getPos(1);
		newTriangle[2*3*i+2] = pos1.x;
		newTriangle[2*3*i+3] = pos1.y;

		glm::vec2 pos2 = listBoids[i].getPos(2);
		newTriangle[2*3*i+4] = pos2.x;
		newTriangle[2*3*i+5] = pos2.y;
	}

	// * Predator
	const float rPredator = 0.15;

	listBoids[nbBoids].pos = glm::vec2(0,0);
	listBoids[nbBoids].velocity = glm::vec2(0,1);

	newTriangle[2*3*nbBoids] = 0;
	newTriangle[2*3*nbBoids+1] = sqrt(3)*sizeBoids/3;
	newTriangle[2*3*nbBoids+2] = -sizeBoids/2;
	newTriangle[2*3*nbBoids+3] = -sqrt(3)*sizeBoids/6;
	newTriangle[2*3*nbBoids+4] = sizeBoids/2 ;
	newTriangle[2*3*nbBoids+5] = -sqrt(3)*sizeBoids/6;

	newColors[nbBoids*3*3] = 1;
	newColors[3+nbBoids*3*3] = 1;
	newColors[6+nbBoids*3*3] = 1;

   	// glfwSetCursorPos(window, width/2, height/2);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	while (!glfwWindowShouldClose(window))
	{
		fps(window, lastTime, lastTimeFPS, n_frame, deltaTime); // Update deltaTime and display fps

        	glEnable(GL_DEPTH_TEST);
        	glDepthFunc(GL_LESS);
        	glEnable(GL_CULL_FACE);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		glBindVertexArray(VAO);

		glUseProgram(shaderProgramBasic);

		// * Predator
		double xpos, ypos;
    		glfwGetCursorPos(window, &xpos, &ypos);
		xpos = xpos*2/width - 1;
		ypos = - ypos*2/height + 1;

		listBoids[nbBoids].velocity = glm::vec2(xpos - listBoids[nbBoids].pos.x, ypos - listBoids[nbBoids].pos.y);
		listBoids[nbBoids].pos += glm::normalize(listBoids[nbBoids].velocity)*glm::vec2(speed*deltaTime,speed*deltaTime);

		for(unsigned short i=0; i<nbBoids; i++){
			for(unsigned short j=0; j<nbBoids; j++){
				if(i!=j){
					float dist = glm::length(listBoids[i].pos - listBoids[j].pos);
					if(dist < rFar){
						if(dist < rNear){
							listBoids[i].dirSeparation += (listBoids[i].pos - listBoids[j].pos);
						}
						else{
							listBoids[i].nbNeighbor+=1;
							listBoids[i].dirAlign += listBoids[j].velocity;
							listBoids[i].dirCohesion += listBoids[j].pos;
						}
					}
				}
			}
		}

		for(unsigned short i=0; i<nbBoids; i++){
			if(glm::length(listBoids[nbBoids].pos-listBoids[i].pos) > rPredator){
				if(listBoids[i].nbNeighbor > 0 ){
					listBoids[i].dirAlign = glm::vec2(listBoids[i].dirAlign.x/listBoids[i].nbNeighbor, listBoids[i].dirAlign.y/listBoids[i].nbNeighbor);
					listBoids[i].dirCohesion = glm::vec2(listBoids[i].dirCohesion.x/listBoids[i].nbNeighbor, listBoids[i].dirCohesion.y/listBoids[i].nbNeighbor);
					listBoids[i].velocity += (listBoids[i].dirCohesion - listBoids[i].pos)*glm::vec2(alignF, alignF) + (listBoids[i].dirAlign - listBoids[i].velocity)*glm::vec2(cohesionF, cohesionF);
				}
				listBoids[i].velocity += listBoids[i].dirSeparation*glm::vec2(sepF, sepF) ;
			}
			else{
				listBoids[i].velocity += (listBoids[i].pos-listBoids[nbBoids].pos) * glm::vec2(sepF, sepF) ;
			}

			listBoids[i].dirSeparation = glm::vec2(0,0);
			listBoids[i].dirAlign = glm::vec2(0,0);
			listBoids[i].dirCohesion = glm::vec2(0,0);
			
			// ! If you change the amount of boids, look here if this still working !
			// * I suppose we can't have more of a third neighbors (*3).
			newColors[1+i*3*3] = (120 + listBoids[i].nbNeighbor * floor(255*3/nbBoids))/255.0f;
			newColors[4+i*3*3] = (120 + listBoids[i].nbNeighbor * floor(255*3/nbBoids))/255.0f;
			newColors[7+i*3*3] = (120 + listBoids[i].nbNeighbor * floor(255*3/nbBoids))/255.0f;
			
			listBoids[i].nbNeighbor=0;

			// * Turn
			if (listBoids[i].pos.x < -1.0f + decayMargin){
				listBoids[i].velocity.x += turnF;
			}
			if (listBoids[i].pos.x > 1.0f - decayMargin){
				listBoids[i].velocity.x -= turnF;
			}
			if (listBoids[i].pos.y < -1.0f + decayMargin){
				listBoids[i].velocity.y += turnF;
			}
			if (listBoids[i].pos.y > 1.0f - decayMargin){
				listBoids[i].velocity.y -= turnF;
			}
			listBoids[i].velocity = glm::normalize(listBoids[i].velocity);
			listBoids[i].pos += glm::normalize(listBoids[i].velocity)*glm::vec2(speed*deltaTime,speed*deltaTime);
		}

		for(unsigned short i=0; i<nbBoids+1; i++){
			glm::vec2 pos0 = listBoids[i].getPos(0);
			newTriangle[2*3*i] = pos0.x;
			newTriangle[2*3*i+1] = pos0.y;

			glm::vec2 pos1 = listBoids[i].getPos(1);
			newTriangle[2*3*i+2] = pos1.x;
			newTriangle[2*3*i+3] = pos1.y;

			glm::vec2 pos2 = listBoids[i].getPos(2);
			newTriangle[2*3*i+4] = pos2.x;
			newTriangle[2*3*i+5] = pos2.y;
		}

		glEnableVertexAttribArray(0);
        	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferBoids);
        	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(boidsVertices), &newTriangle);

		glEnableVertexAttribArray(1);
        	glBindBuffer(GL_ARRAY_BUFFER, colorbufferBoids);
        	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(boidsColors), &newColors);

		glDrawArrays(GL_TRIANGLES, 0, 3*(nbBoids+1));

		unbindBuffer(1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	glDeleteProgram(shaderProgramBasic);

	glDeleteBuffers(1, &vertexbufferBoids);
	glDeleteBuffers(1, &colorbufferBoids);


	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
