#version 450

in vec3 frag_position;
in vec3 frag_normal;

out vec4 frag_color;

uniform float h;
uniform vec4 cam_pos;
uniform sampler2D floor_texture;

#define LIGHTS_COUNT 3
vec3 Lights_pos[LIGHTS_COUNT];

float CalcLight(vec3 light_pos, vec3 normal, vec3 frag)
{
    vec3 lightDir = normalize(light_pos - frag);

    normal = -1 * normal;
    float diff = max(dot(normal, lightDir), 0.0);

    return diff * 0.3;
} 

void main () {
    Lights_pos[0] = vec3(0.0, 3.0, -3.0);
    Lights_pos[1] = vec3(-2.0, 2.0, 1.0);
    Lights_pos[2] = vec3(2.0, 3.0, 3.0);

    float kd = 0.0;
    float floor_kd = 0.0;
    for(int i = 0; i < LIGHTS_COUNT; i++)
        kd += CalcLight(Lights_pos[i], frag_normal, frag_position);

    vec3 incident = normalize(frag_position - cam_pos.xyz);
    vec3 refraction = refract(incident, frag_normal, 0.85);

    vec3 floor_hit_point = frag_position + refraction * ((2 - frag_position.y) / refraction.y);
    vec2 floor_texture_coord = ((floor_hit_point + vec3(h, 0.0, h)) / (2.0 * h)).xz;

    if (floor_texture_coord.y >= 0.0 && 
        floor_texture_coord.y <= 1.0 &&
        floor_texture_coord.x >= 0.0 &&
        floor_texture_coord.x <= 1.0) {

        floor_kd = 0.0;
        for(int i = 0; i < LIGHTS_COUNT; i++)
            floor_kd += 2.0 * CalcLight(Lights_pos[i], vec3(0.0, -1.0, 0.0), floor_hit_point);
    }

    if (floor_texture_coord.y < 0.0) {
        floor_hit_point = frag_position + refraction * ((-h - frag_position.z) / refraction.z);
        floor_texture_coord = ((floor_hit_point + vec3(h, h, 0.0)) / (2.0 * h)).xy;

        floor_kd = 0.0;
        for(int i = 0; i < LIGHTS_COUNT; i++)
            floor_kd += CalcLight(Lights_pos[i], vec3(0.0, 0.0, -1.0), floor_hit_point);
    }
    if (floor_texture_coord.x < 0.0) {
        floor_hit_point = frag_position + refraction * ((-h - frag_position.x) / refraction.x);
        floor_texture_coord = ((floor_hit_point + vec3(0.0, h, h)) / (2.0 * h)).yz;

        floor_kd = 0.0;
        for(int i = 0; i < LIGHTS_COUNT; i++)
            floor_kd += CalcLight(Lights_pos[i], vec3(-1.0, 0.0, 0.0), floor_hit_point);
    }

    if (floor_texture_coord.y > 1.0) {
        floor_hit_point = frag_position + refraction * ((h - frag_position.z) / refraction.z);
        floor_texture_coord = ((floor_hit_point + vec3(h, h, 0.0)) / (2.0 * h)).xy;

        floor_kd = 0.0;
        for(int i = 0; i < LIGHTS_COUNT; i++)
            floor_kd += CalcLight(Lights_pos[i], vec3(0.0, 0.0, 1.0), floor_hit_point);
    }

    if (floor_texture_coord.x > 1.0) {
        floor_hit_point = frag_position + refraction * ((h - frag_position.x) / refraction.x);
        floor_texture_coord = ((floor_hit_point + vec3(0.0, h, h)) / (2.0 * h)).yz;

        floor_kd = 0.0;
        for(int i = 0; i < LIGHTS_COUNT; i++)
            floor_kd += CalcLight(Lights_pos[i], vec3(1.0, 0.0, 0.0), floor_hit_point);
    }

    vec3 floor_texel = texture(floor_texture, floor_texture_coord).xyz;

    frag_color = vec4(kd * (0.8 * vec3(0.0, 0.149, 0.182) + 1.3 * floor_kd * floor_texel), 1.0);
}
