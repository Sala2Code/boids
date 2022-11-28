#version 330 core

out vec4 color;

in vec3 aColor;

void main(){
	color.rgb = aColor;
	color.a = 1;	
}
