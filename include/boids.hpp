#ifndef BOIDS_HPP
#define BOIDS_HPP

#include "../common/include.hpp"

const int nbBoids = 200;
const float sizeBoids = 0.03;

struct Boids{
    /*
    Boids at the beginning
           0
           ^
          / \
       1 /___\ 2
    */
    glm::vec2 pos; // Gravity's center
    glm::vec2 velocity;
    int nbNeighbor = 0;
    glm::vec2 dirSeparation = glm::vec2(0,0);
    glm::vec2 dirAlign = glm::vec2(0,0);
    glm::vec2 dirCohesion = glm::vec2(0,0);

    Boids(){

        float x = (rand()%100 - 50) / 50.0f; 
        float y = (rand()%100 - 50) / 50.0f; 

        float vx = (rand()%100 - 50) / 50.0f; 
        float vy = (rand()%100 - 50) / 50.0f; 

        this->pos = glm::vec2(x, y);
        this->velocity = glm::vec2(vx, vy);

    };

    // Get the vertex position
    glm::vec2 getPos(int sommet);

};

#endif