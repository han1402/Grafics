#version 330 core

out vec4 color;

in Ver_out {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 normal;
    mat3 tbn;
}v_in;

struct Material {
    float shin;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap;
};
 
struct Light{
    vec3 light_pos;
    vec3 view_pos;
};

uniform Material material;
uniform Light light;

void main()
{
    //vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
    //normal = normalize(normal * 2.0f - 1.0f);

    vec3 normal = v_in.normal;

    vec3 diff_color = texture(material.diffuse, v_in.tex_coords).rgb;
    vec3 ambient = 0.3f * diff_color;


    vec3 light_dir = normalize(light.light_pos - v_in.frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = diff * diff_color;


    vec3 view_dir = normalize(light.view_pos - v_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.00000001f), material.shin);
    vec3 specular =  texture(material.specular, v_in.tex_coords).rgb * spec;//* texture(material.specular, v_in.tex_coords).r


    float distance = length(light.light_pos -  v_in.frag_pos);
    float attenuation = 1.0 / (1.0f + 0.04f * distance + 0.01f * distance * distance);

    vec3 result = (ambient + diffuse + specular) * attenuation;
    color = vec4(result, 1.0f);


}







