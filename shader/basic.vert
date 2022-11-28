#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 inColor;

out vec3 aColor;

void main(){
	aColor = inColor;
	gl_Position = vec4(aPos.x, aPos.y, 0, 1);
}
