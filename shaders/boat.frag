#version 450

in vec3 frag_position;
in vec3 frag_normal;

out vec4 frag_color;

#define LIGHTS_COUNT 3
vec3 Lights_pos[LIGHTS_COUNT];

float CalcLight(vec3 light_pos, vec3 normal, vec3 frag)
{
    vec3 lightDir = normalize(light_pos - frag);

    float diff = max(dot(normal, lightDir), 0.0);

    return diff * 0.33;
} 

void main () {
  Lights_pos[0] = vec3(0.0, 3.0, -3.0);
    Lights_pos[1] = vec3(-2.0, 2.0, 1.0);
    Lights_pos[2] = vec3(2.0, 3.0, 3.0);

    float kd = 0.0;
    for(int i = 0; i < LIGHTS_COUNT; i++)
        kd += CalcLight(Lights_pos[i], frag_normal, frag_position);

    frag_color = kd * vec4(0.15, 0.08, 0.03, 1.0) + vec4(0.05, 0.01, 0.01, 1.0);
}
