#ifndef RENDER_H
#define RENDER_H

#include "my_stb.h"
#include "model.h"

int my_drow_light(GLuint vao, GLuint sh, glm::mat4 model,  glm::mat4 view, glm::mat4 projection);
GLuint my_set_light_vao();
int my_drow_glass_box(GLuint glass_VAO, GLuint sh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint sky_texture, glm::vec3 cam_pos);
GLuint set_glass_vao();
int drow_sky_box(GLuint sky_VAO, GLuint sh, GLuint sky_tex, glm::mat4 model, glm::mat4 view, glm::mat4 projection);
GLuint set_sky_box_vao();
GLuint set_sky_box_tex();

struct myMaterial{
    GLuint diffuse_color;
    GLuint specular_map;
    GLuint normal_map;
    GLuint shadow_map;
    glm::mat4 sh_matrix;
    bool blue_ex;
    GLuint paralax;
    bool if_paralax;
};


GLuint set_sky_box_tex( ){
    GLuint sky_texture;
    const char *  textured_cube[] = {
        "./textures/skybox/posx.jpg",        "./textures/skybox/negx.jpg",
        "./textures/skybox/posy.jpg",        "./textures/skybox/negy.jpg",
        "./textures/skybox/posz.jpg",        "./textures/skybox/negz.jpg"     
        };
    glGenTextures(1, &sky_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture);
    unsigned char *data;
    int tex_width , tex_height, n; 
    for(unsigned int i = 0; i < 6; i++)
    {    
        data = stbi_load(textured_cube[i], &tex_width, &tex_height, &n, 0);
        if(data == NULL)
            std::cout << "error SKYbox texture" << std::endl;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
    return sky_texture;
}

GLuint set_sky_box_vao(){
    float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,    1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f,  1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,     1.0f, -1.0f,  1.0f,     1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,     1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,     1.0f, -1.0f,  1.0f,    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,     1.0f,  1.0f, -1.0f,    1.0f,  1.0f,  1.0f,     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,    -1.0f,  1.0f, -1.0f,    -1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,     1.0f, -1.0f, -1.0f,    -1.0f, -1.0f,  1.0f,     1.0f, -1.0f,  1.0f
    };
    GLuint sky_VAO;
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &sky_VAO);
    glBindVertexArray(sky_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return sky_VAO;
}

int drow_sky_box(GLuint sky_VAO, GLuint sh, GLuint sky_tex, glm::mat4 model, glm::mat4 view, glm::mat4 projection){
glDepthFunc(GL_LEQUAL); 
        glUseProgram(sh);  
        glBindVertexArray(sky_VAO);
        view = glm::mat4(glm::mat3(view));  
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, sky_tex);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); 
glDepthFunc(GL_LESS); 
    return 0;
}





GLuint set_glass_vao(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_for_glass), vertices_for_glass, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return VAO;
}

int my_drow_glass_box(GLuint glass_VAO, GLuint sh, glm::mat4 model, glm::mat4 view, glm::mat4 projection, GLuint sky_texture, glm::vec3 cam_pos){
        glUseProgram(sh);  
        glBindVertexArray(glass_VAO);
        glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(sh, "camera_pos"), 1, glm::value_ptr(cam_pos));   
        glBindTexture(GL_TEXTURE_CUBE_MAP, sky_texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0); 
    return 0;
}





GLuint my_set_light_vao(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_ver), cube_ver, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

    return VAO;
}


int my_drow_light(GLuint vao, GLuint sh, glm::mat4 model,  glm::mat4 view, glm::mat4 projection){
    glUseProgram(sh);

    glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    return 0;
}


glm::vec3 sort_camera_pos;
int func_cmp( const void * val1, const void * val2 ){
    glm::vec3 a1 = *(glm::vec3 *)(val1);
    glm::vec3 a2 = *(glm::vec3 *)(val2);
    float dist1, dist2;
dist1 = glm::length(sort_camera_pos - a1); 
dist2 = glm::length(sort_camera_pos - a2);
    if(dist1 > dist2)
        return 0;
    return 1;
}

int drow_bilbords( GLuint sh , GLuint vao, glm::mat4 view, glm::mat4 projection, glm::vec3 camera_pos , GLuint texture){
     glm::vec3 bilb_pos[] = {
  glm::vec3(3.2f,  0.5f,  2.0f),   glm::vec3(3.2f,  0.5f,  3.0f),     glm::vec3(3.2f,  0.5f,  4.0f),    glm::vec3(3.2f,  0.5f,  5.0f),  glm::vec3(3.2f,  0.5f,  6.0f) };
    sort_camera_pos = camera_pos;
    qsort(&bilb_pos, 5, sizeof(glm::vec3), func_cmp);
    glUseProgram(sh);  
    for(int i = 0; i < 5; i++){
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, bilb_pos[i]);
        glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));      
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(glGetUniformLocation(sh, "my_texture"), 0);
        glBindVertexArray(vao); 
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0); 
    }
    return 0;
}




GLuint set_post_vao(){
    GLuint VBO, VAO;
    float quad_vertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,         1.0f,  1.0f,  1.0f, 1.0f   };
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return VAO;
}

GLuint set_post_buffer(GLuint &tex_color_buffer, int w, int h){
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);  
    glGenTextures(1, &tex_color_buffer);
    glBindTexture(GL_TEXTURE_2D, tex_color_buffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_buffer, 0); 
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return framebuffer;
}


int drow_post_effect(GLuint texture, GLuint vao, GLuint sh){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(sh);  
    glBindVertexArray(vao);
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(sh, "screen_tex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);  
    glEnable(GL_DEPTH_TEST);
    return 0;
}


GLuint bilb_setvao(){
    GLuint VAO;
    GLuint VBO;
    float trvertices[] = {
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,        0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  1.0f,  1.0f,        1.0f,  0.5f,  0.0f,  1.0f,  0.0f    };
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trvertices), trvertices, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    return VAO;
}



GLuint loaad_bilboard_texture(const char * path){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    int  tex_width, tex_height, np;
    unsigned char * image = stbi_load(path, &tex_width, &tex_height, &np, 0);
    if(image == NULL)
        std::cout << "error openb texture" << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

int load_textures(GLuint &diffuse, GLuint &specular, GLuint &normal, GLuint &height){
    int  tex_width, tex_height, n;
    unsigned char * image;


    glGenTextures(1, &diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    image = stbi_load("./textures/Tiles01/Tiles_01_basecolor.jpg", &tex_width, &tex_height, &n, 0);
    if(image == NULL){
        std::cout << "ERROR OPEN diffuse texture" << std::endl;
        return -1;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    glGenTextures(1, &specular);
    glBindTexture(GL_TEXTURE_2D, specular);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    image = stbi_load("./textures/Tiles01/Tiles_01_glossiness.jpg", &tex_width, &tex_height, &n, 0);
    if(image == NULL){
        std::cout << "ERROR OPEN specular texture " << std::endl;
        return -1;
    }
    //std::cout << n << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,  GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
//////////////////////////////
    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    image = stbi_load("./textures/Tiles01/Tiles_01_normalOgl.jpg", &tex_width, &tex_height, &n, 0);
    if(image == NULL){
        std::cout << "ERROR OPEN normal texture " << std::endl;
        return -1;
    }
    //std::cout << n << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,  GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    glGenTextures(1, &height);
    glBindTexture(GL_TEXTURE_2D, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    image = stbi_load("./textures/Tiles01/Tiles_01_height.jpg", &tex_width, &tex_height, &n, 0);
    if(image == NULL){
        std::cout << "ERROR OPEN normal texture " << std::endl;
        return -1;
    }
    //std::cout << n << std::endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0,  GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    return 0;
}
 
/*GLfloat plane_vertices1OLD[] = {
         10.0f, -0.0f,  10.0f, 0.0f, 0.1f, 0.0f,  5.0f, 0.0f,
        -10.0f, -0.0f,  10.0f, 0.0f, 0.1f, 0.0f,  0.0f, 0.0f,
        -10.0f, -0.0f, -10.0f, 0.0f, 0.1f, 0.0f,  0.0f, 5.0f,
         10.0f, -0.0f,  10.0f, 0.0f, 0.1f, 0.0f,  5.0f, 0.0f,
        -10.0f, -0.0f, -10.0f, 0.0f, 0.1f, 0.0f,  0.0f, 5.0f,
         10.0f, -0.0f, -10.0f, 0.0f, 0.1f, 0.0f,  5.0f, 5.0f
    };  */

GLfloat plane_vertices1[] = { 
     10.0f, 0.0 ,  10.0f , 0 , 1 , 0 , 5 , 0 , -1 , 0 , 0 , 
    -10.0f, 0.0 , -10.0f , 0 , 1 , 0 , 0 , 5 , -1 , 0 , 0 , 
    -10.0f, 0.0 ,  10.0f , 0 , 1 , 0 , 0 , 0 , -1 , 0 , 0 , 
     10.0f, 0.0 ,  10.0f , 0 , 1 , 0 , 5 , 0 , -1 , 0 , 0 , 
     10.0f, 0.0 , -10.0f , 0 , 1 , 0 , 5 , 5 , -1 , 0 , 0 , 
    -10.0f, 0.0 , -10.0f , 0 , 1 , 0 , 0 , 5 , -1 , 0 , 0  } ;

GLuint set_plane_vao1(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices1), plane_vertices1, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}

int drow_plane1(int ntr, GLuint sh, GLuint vao, glm::vec3 camera_pos, glm::vec3 light_pos, myMaterial local_material, glm::mat4 model,  glm::mat4 view, glm::mat4 projection){
        glUseProgram(sh);

        glUniformMatrix4fv(glGetUniformLocation(sh, "model"), 1, GL_FALSE, glm::value_ptr(model));     
        glUniformMatrix4fv(glGetUniformLocation(sh, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sh, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
         glUniformMatrix4fv(glGetUniformLocation(sh, "shadow_m"), 1, GL_FALSE, glm::value_ptr(local_material.sh_matrix));

        glUniform3fv(glGetUniformLocation(sh, "light.view_pos"), 1, glm::value_ptr(camera_pos));        
        glUniform3fv(glGetUniformLocation(sh, "light.light_pos"), 1, glm::value_ptr(light_pos));   

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, local_material.diffuse_color);
        glUniform1i(glGetUniformLocation(sh, "material.diffuse"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, local_material.specular_map);
        glUniform1i(glGetUniformLocation(sh, "material.specular"), 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, local_material.normal_map);
        glUniform1i(glGetUniformLocation(sh, "material.normals_map"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, local_material.shadow_map);
        glUniform1i(glGetUniformLocation(sh, "material.sh_map"), 3);
        glUniform1f(glGetUniformLocation(sh,"material.shin"), 0.5f * 128.0f);

        glUniform1i(glGetUniformLocation(sh,"blue_ex"), local_material.blue_ex);
        glUniform1i(glGetUniformLocation(sh,"if_paralax"), local_material.if_paralax);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, local_material.paralax);
        glUniform1i(glGetUniformLocation(sh, "material.paralax"), 4);

        glBindVertexArray(vao); 
        glDrawArrays(GL_TRIANGLES, 0, ntr);
        glBindVertexArray(0); 
        return 0;
}

GLuint set_cubes_vao_nt(){
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_for_cube_nornals_tn), vertices_for_cube_nornals_tn, GL_STATIC_DRAW); 
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1); 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(2); 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8*sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}


int shadow_buffer_and_text(GLuint &buffer, GLuint &texture, int shadow_w, int shadow_h){
    glGenFramebuffers(1, &buffer);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,  shadow_w, shadow_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glm::vec4 bord_c = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(bord_c));
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return 0;
}



#endif
