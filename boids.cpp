#include "include/boids.hpp"

#define PI 3.1415926535

// * On tourne selon un angle !
glm::vec2 Boids::getPos(int sommet){
    float angle = (sommet==0) ? 0 : (sommet==1) ? 2*PI/3 : 4*PI/3;
    
    angle+=atan(this->velocity.y / this->velocity.x);
    float x = cos(angle)*sqrt(3)*sizeBoids/3 + this->pos.x;
    float y = sin(angle)*sqrt(3)*sizeBoids/3 + this->pos.y;
    
    return glm::vec2(x, y);
}