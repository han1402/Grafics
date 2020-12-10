#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
//layout (location = 3) in vec3 tangens;
//layout (location = 4) in vec3 bitangens;

out Ver_out {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 normal;
    mat3 tbn;
} ver_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    ver_out.frag_pos = vec3(model * vec4(position, 1.0));   
    ver_out.tex_coords = tex_coord;
    
    mat3 normal_matrix = transpose(inverse(mat3(model)));
        

ver_out.normal = normalize(normal_matrix * normal);


    gl_Position = projection * view * model * vec4(position, 1.0);
} 


