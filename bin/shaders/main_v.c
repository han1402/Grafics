#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 tangens;

out Verh_out {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 normal;
    mat3 tbn_m;
    vec4 sh_pos;
    mat3 tbn_inverse;
} verh_out;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 shadow_m;

void main()
{
    verh_out.frag_pos = vec3(model * vec4(position, 1.0));   
    verh_out.tex_coords = tex_coord;
    
    mat3 normal_m = transpose(inverse(mat3(model)));
    vec3 Nv = normalize(normal_m * normal);
    vec3 Tv = normalize(normal_m * tangens);
    vec3 Bv = normalize(cross(Nv, Tv));
    verh_out.tbn_m  = mat3(Tv, Bv, Nv);
    verh_out.tbn_inverse = transpose(verh_out.tbn_m);

    verh_out.sh_pos = shadow_m * vec4(verh_out.frag_pos, 1.0);
    verh_out.normal = normalize(normal_m * normal);
    gl_Position = projection * view * model * vec4(position, 1.0);
} 


