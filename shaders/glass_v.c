#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

out vec3 o_normal;
out vec3 o_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    o_normal = mat3(transpose(inverse(model))) * normal;
    o_position = vec3(model * vec4(pos, 1.0));
    gl_Position = projection * view * model * vec4(pos, 1.0);
}
