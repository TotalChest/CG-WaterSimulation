#version 450

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_diffuse_color;
out vec2 frag_texture_coord;

uniform vec4 text_coord;
uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 proj_mat;

void main () {
	frag_position = vec3(model_mat * vec4(vertex_position, 1.0f));
   	frag_normal = normalize(mat3(transpose(inverse(model_mat))) * vertex_normal);
   	vec2 text_pos = (vertex_position.xy + vec2(1.0, 1.0)) / 2.0;

   	if(text_pos[0] == 0)
   		frag_texture_coord[0] = text_coord[0];
   	else
   		frag_texture_coord[0] = text_coord[2];
   	if(text_pos[1] == 0)
   		frag_texture_coord[1] = text_coord[1];
   	else
   		frag_texture_coord[1] = text_coord[3];

   gl_Position = proj_mat * view_mat * model_mat * vec4(vertex_position, 1.0);
}
