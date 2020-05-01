#version 450

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_texture_coord;

out vec4 frag_color;

uniform sampler2D floor_texture;

#define LIGHTS_COUNT 2
vec3 Lights_pos[LIGHTS_COUNT];

float CalcLight(vec3 light_pos, vec3 normal, vec3 frag)
{
    vec3 lightDir = normalize(light_pos - frag);

    float diff = max(dot(normal, lightDir), 0.0);

    return diff * 0.5;
} 

void main () {
	Lights_pos[0] = vec3(2.0, 1.0, 2.5);
   	Lights_pos[1] = vec3(-4.0, 6.0, -5.0);

   	float kd = 0.0;
   	for(int i = 0; i < LIGHTS_COUNT; i++)
        kd += CalcLight(Lights_pos[i], frag_normal, frag_position);

   	vec3 texel = texture(floor_texture, frag_texture_coord).xyz;

   	frag_color =  kd * vec4(texel, 1.0) + vec4(0.05, 0.05, 0.05, 1.0);
}
