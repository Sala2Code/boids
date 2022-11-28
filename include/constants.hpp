#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP 

// Not const for extern 
int width = 800;
int height = 800;

// * fps
double lastTime = glfwGetTime();
double lastTimeFPS = lastTime;
unsigned int n_frame = 0;
float deltaTime = 0;

float speed = 1;
const float rNear = 0.05;
const float rFar = 0.12;

const float alignF = 0.05;
const float sepF = 0.5;
const float cohesionF = 0.5;
const float turnF = 0.05;
const float decayMargin = 0.1;

#endif