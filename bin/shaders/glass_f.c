#version 330 core
out vec4 FragColor;

in vec3 o_normal;
in vec3 o_position;

uniform vec3 camera_pos;
uniform samplerCube skybox;

void main()
{             
    float ratio = 1.00 / 1.52;
    vec3 i = normalize(o_position - camera_pos);
    vec3 r = refract(i, normalize(o_normal), ratio);
    FragColor = vec4(mix(vec3(0.6, 0.901, 1.0)  ,texture(skybox, r).rgb, 0.7), 1.0);
}
