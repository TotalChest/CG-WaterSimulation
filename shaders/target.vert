#version 450

in vec3 vertex_position;

uniform mat4 model_mat;

void main () {
   gl_Position = model_mat * vec4(vertex_position, 1.0);
}