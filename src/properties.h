#ifndef PROPERTIES_H
#define PROPERTIES_H

float h = 4.0;
bool carcass = false;
bool keys[1024]; 
bool boat_ex = false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
double lastX, lastY;
bool firstMouse = true;
GLfloat yaw   = -90.0f;
GLfloat pitch = -45.0f;
const float PI = 3.1415926535f;
static const GLsizei WIDTH = 1600, HEIGHT = 900;

#endif