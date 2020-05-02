#version 450

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 frag_position;
out vec3 frag_normal;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main () {
	frag_position = vec3(model_mat * vec4(vertex_position, 1.0f));
   frag_normal = normalize(mat3(transpose(inverse(model_mat))) * vertex_normal);
   gl_Position = proj_mat * view_mat * model_mat * vec4(vertex_position, 1.0);
}
