#version 330 core

out vec4 color;

in Verh_out {
    vec3 frag_pos;
    vec2 tex_coords;
    vec3 normal;
    mat3 tbn_m;
    vec4 sh_pos;
    mat3 tbn_inverse;
}v_in;

struct Material {
    float shin;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normals_map;
    sampler2D sh_map;
    sampler2D paralax;
};
 

struct Light{
    vec3 light_pos;
    vec3 view_pos;
};

uniform Material material;
uniform Light light;
uniform bool blue_ex;
uniform bool if_paralax;

float c_shadow(vec3 normal, vec3 light_dir){
    vec3 project = v_in.sh_pos.xyz / v_in.sh_pos.w;
    project = project * 0.5 + 0.5;
    if(project.z > 1.0){
        return 0.0f;
    }
    float realdepth = project.z;
    float shadow = 0.0f;
    float bias = max(0.12f * (1.0f - abs(dot(normal, light_dir))), 0.008f);
    vec2 texel_size = 1.0 / textureSize(material.sh_map, 0);
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            float tmp = texture(material.sh_map, project.xy + vec2(x, y) * texel_size).r;
            if(realdepth - bias > tmp){
                shadow += 1.0;
            }
        }
    }
    shadow /= 9.0;
//  if(real_depth - bias > depth)   shadow = 1.0;   els    shadow = 0.0f;
    return shadow;
}



vec2 parallax_mapping(vec2 input_tex_coords, vec3 my_view_dir, int N1, int N2){
    float height_pow = 0.012f; 
    float num_layers = N1; //10
    float layer_depth = 1.0f / num_layers;
    float current_depth = 0.0f;
    vec2 vec_shift = (my_view_dir.xy / 1.0f) * height_pow; 
    vec2 delta_coords = vec_shift / num_layers;
    vec2  current_tex_coords  = input_tex_coords;
    float current_depth_value = 1.0f - texture(material.paralax, current_tex_coords).r;  
    while(current_depth < current_depth_value){
        current_tex_coords -= delta_coords;
        current_depth_value = 1.0f - texture(material.paralax, current_tex_coords).r;  
        current_depth += layer_depth;  
    }
    delta_coords *= 0.5f;
    layer_depth *= 0.5f;
    current_tex_coords += delta_coords;
    current_depth -= layer_depth;  
    for(int iter = N2; iter > 0; iter--){
        current_depth_value = 1.0f - texture(material.paralax, current_tex_coords).r;  
        delta_coords *= 0.5f;
        layer_depth *= 0.5f;
        if(current_depth_value > current_depth){
            current_tex_coords -= delta_coords;
            current_depth += layer_depth;
        }else{
            current_tex_coords += delta_coords;
            current_depth -= layer_depth;
        }
    }
    return current_tex_coords; 
}


void main(){

    vec2 new_coords;
    if(if_paralax){
        vec3 view_dir_tbn = normalize(v_in.tbn_inverse * normalize(light.view_pos - v_in.frag_pos));
        new_coords = parallax_mapping(v_in.tex_coords,  view_dir_tbn, 10, 5);
    }else{
        new_coords = v_in.tex_coords;
    }


    vec3 normal = texture(material.normals_map, new_coords).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(v_in.tbn_m * normal);  

    vec3 diff_color = texture(material.diffuse, new_coords).rgb;
    vec3 ambient = 0.4f * diff_color;


    vec3 light_dir = normalize(light.light_pos - v_in.frag_pos);
    float diff = max(dot(normal, light_dir), 0.0f);
    vec3 diffuse = diff * diff_color;


    vec3 view_dir = normalize(light.view_pos - v_in.frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.00000001f), material.shin);
    vec3 specular =  texture(material.specular, new_coords).rgb * spec;//* texture(material.specular, new_coords).r

    float distance = length(light.light_pos -  v_in.frag_pos);
    float attenuation = 1.0 / (1.0f + 0.007f * distance + 0.01f * distance * distance);

    float shadow_par = c_shadow(v_in.normal, light_dir);
    vec3 result = (ambient + (diffuse + specular)*(1.0f - shadow_par)) * attenuation;

    if(blue_ex){
        color = vec4(result.r, result.g, 1.0f, 1.0f);
    }else{
        color = vec4(result, 1.0f);
    }
}




    //vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
    //normal = normalize(normal * 2.0f - 1.0f);
    //vec3 normal = v_in.normal;


