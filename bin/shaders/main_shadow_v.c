#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 tangens;

uniform mat4 model;
uniform mat4 shadow_m;

void main(){

    gl_Position = shadow_m * model * vec4(position, 1.0);

} 


