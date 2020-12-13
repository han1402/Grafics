#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "model.h"
#include "my_render.h"
#include "camera.h"

myCamera camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
int my_camera_step();

int main(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    camera.screen_width = 1340;
    camera.screen_height = 800;
    GLFWwindow* window = glfwCreateWindow(camera.screen_width, camera.screen_height, "Openglmy", nullptr, nullptr);
    if (window == nullptr){
	    std::cout << "@error create GLFW window" << std::endl;
	    glfwTerminate();
	    return 0;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);  
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();  //glGetError();
    if (err != GLEW_OK){
        std::cout << "@error glewInit()  " << glGetError() << std::endl;
        return 0;
    }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
/*********************************************************************************/
camera.yaw   = -90.0f;
camera.pitch = 0.0f;
camera.last_x = 400;
camera.last_y = 300;
camera.camera_pos   = glm::vec3(3.0f, 1.0f,  3.0f);
camera.camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
camera.camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);
camera.first_mouse = true;
camera.last_time = glfwGetTime();
camera.delta_time = 0;
for(int i = 0; i < 4; i++){
    camera.keystate[i] = false;
}
/***********************************************************************************/
    int wwidth, wheight;
    glfwGetFramebufferSize(window, &wwidth, &wheight);
    glViewport(0, 0, wwidth, wheight);
    camera.screen_width = wwidth;
    camera.screen_height = wheight;
/***************************************SHADERS*************************************/
GLuint light_SH = compl_shader("./shaders/c_light_v.c", "./shaders/c_light_f.c");
GLuint light_VAO = my_set_light_vao();

GLuint sky_SH = compl_shader("./shaders/sky_v.c", "./shaders/sky_f.c");
GLuint sky_tex = set_sky_box_tex();
GLuint sky_VAO = set_sky_box_vao();

GLuint glass_SH = compl_shader("./shaders/glass_v.c", "./shaders/glass_f.c");
GLuint glass_vao = set_glass_vao();

GLuint bilb_vao =  bilb_setvao();
GLuint bilb_tex =  loaad_bilboard_texture("./textures/CrackScreen.png");
GLuint bilb_SH = compl_shader("./shaders/bilb_v.c", "./shaders/bilb_f.c");

camera.peff = false;
GLuint post_VAO = set_post_vao();
GLuint tex_color_buffer;
GLuint post_BUFF = set_post_buffer(tex_color_buffer, camera.screen_width , camera.screen_height);
GLuint post_SH = compl_shader("./shaders/eff_post_v.c", "./shaders/eff_post_f.c");

GLuint main_SH = compl_shader("./shaders/main_v.c", "./shaders/main_f.c");
GLuint plane_vao = set_plane_vao1();
myMaterial material;
material.blue_ex = false;
camera.if_paralax = false;
load_textures(material.diffuse_color , material.specular_map , material.normal_map, material.paralax);


GLuint cubes_VAO = set_cubes_vao_nt();
GLuint shadow_SH = compl_shader("./shaders/main_shadow_v.c", "./shaders/main_shadow_f.c");
GLuint shadow_buffer;
shadow_buffer_and_text(shadow_buffer, material.shadow_map ,  camera.screen_width,  camera.screen_height);
//std::cout << "YES" << main_SH << plane_vao << material.diffuse_color << std::endl;
/***********************************************************************************/
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        my_camera_step();
        glClearColor(0.0f, 0.1f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glViewport(0, 0, wwidth, wheight);

        glm::mat4 model(1.0f);
        glm::mat4 view = glm::lookAt(camera.camera_pos, camera.camera_pos + camera.camera_front, camera.camera_up);
        glm::mat4 projection = glm::perspective(0.78539f, camera.screen_width / camera.screen_height, 0.1f, 100.0f);
        glm::vec3 light_position = glm::vec3(3.5f + 5.0f*glm::sin(1.5f*glfwGetTime()), 3.5f, 2.0f);
///INIT

        
        glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);
//    glViewport(0, 0, wwidth, wheight);
        glViewport(0, 0, camera.screen_width, camera.screen_height);
        glClear(GL_DEPTH_BUFFER_BIT);
        float ap = 1.0f, bp = 12.0f;
        glm::mat4 light_projection = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, ap, bp);
        glm::mat4 light_view = glm::lookAt(light_position, glm::vec3(3.8f, 0.0f, -0.2f), glm::normalize(glm::vec3( 0.0f, 1.0f,  0.0f)));
        glm::mat4 shadow_m = light_projection * light_view;
        material.sh_matrix = shadow_m;
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        for(int i = 0; i < 3; i++){
            model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f - 1.5f*i , 1.0f + 0.1f*i, -1.0f - 0.1f*i ));//glm::vec3(5.0f - 1.2f*i , 1.0f + 0.1f*i, -1.0f - 0.1f*i )
            model = glm::scale(model, glm::vec3(0.4f + 0.1f*i));
            drow_plane1(36, shadow_SH, cubes_VAO, camera.camera_pos, light_position, material, model,  view, projection);
        }
        glCullFace(GL_BACK); 
        glDisable(GL_CULL_FACE);  
        model = glm::mat4(1.0f);
        drow_plane1(6, shadow_SH, plane_vao, camera.camera_pos, light_position, material, model,  view, projection);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, wwidth, wheight);
///SHADOW
        if(camera.peff){
            glBindFramebuffer(GL_FRAMEBUFFER, post_BUFF);
    glViewport(0, 0, wwidth, wheight);
//        glViewport(0, 0, camera.screen_width, camera.screen_height);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
        }

        model = glm::translate(model, light_position);
        model = glm::scale(model, glm::vec3(0.1f));
        my_drow_light(light_VAO, light_SH, model, view, projection);
        model = glm::mat4(1.0f);

        my_drow_glass_box( glass_vao,  glass_SH, glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 3.0f, 4.0f)), view,  projection, sky_tex,  camera.camera_pos);


glEnable(GL_STENCIL_TEST);
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
glStencilFunc(GL_ALWAYS, 1, 0xFF); 
glStencilMask(0xFF);
        for(int i = 0; i < 3; i++){
            model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f - 1.5f*i , 1.0f + 0.1f*i, -1.0f - 0.1f*i ));
            model = glm::scale(model, glm::vec3(0.4f + 0.1f*i));
            drow_plane1(36, main_SH, cubes_VAO, camera.camera_pos, light_position, material, model,  view, projection);
        }
glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
glStencilMask(0x00);
material.blue_ex = true;
        for(int i = 0; i < 3; i++){
            model = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f - 1.5f*i , 1.0f + 0.1f*i, -1.0f - 0.1f*i ));
            model = glm::scale(model, glm::vec3(0.4f + 0.1f*i));
            model = glm::scale(model, glm::vec3(1.05f));
            drow_plane1(36, main_SH, cubes_VAO, camera.camera_pos, light_position, material, model,  view, projection);
        }
material.blue_ex = false;
glStencilMask(0xFF);
glDisable(GL_STENCIL_TEST);

            model = glm::mat4(1.0f);
            material.if_paralax = camera.if_paralax;
            drow_plane1(6, main_SH, plane_vao, camera.camera_pos, light_position, material, model,  view, projection);
            material.if_paralax = false;

        drow_sky_box(sky_VAO, sky_SH, sky_tex,  model,  view, projection);
        drow_bilbords(  bilb_SH , bilb_vao, view, projection,   camera.camera_pos, bilb_tex);
        if(camera.peff){
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            drow_post_effect( tex_color_buffer ,  post_VAO,  post_SH);
    glViewport(0, 0, wwidth, wheight);
        }
        glfwSwapBuffers(window);
    }
    glfwTerminate();
/***********************************************************************************/
    return 0;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, GL_TRUE);
    if(key == GLFW_KEY_W){
        if(action == GLFW_PRESS){
            camera.keystate[1] = true;
        }else if(action == GLFW_RELEASE){
            camera.keystate[1] = false;
        }
    }
    if(key == GLFW_KEY_S){
        if(action == GLFW_PRESS){
            camera.keystate[3] = true;
        }else if(action == GLFW_RELEASE){
            camera.keystate[3] = false;
        }
    }
    if(key == GLFW_KEY_A){
        if(action == GLFW_PRESS){
            camera.keystate[0] = true;
        }else if(action == GLFW_RELEASE){
            camera.keystate[0] = false;
        }
    }
    if(key == GLFW_KEY_D){
        if(action == GLFW_PRESS){
            camera.keystate[2] = true;
        }else if(action == GLFW_RELEASE){
            camera.keystate[2] = false;
        }
    }
    if(key == GLFW_KEY_F){
        if(action == GLFW_PRESS){
            if(camera.peff == false){
                camera.peff = true;
            }else{
                camera.peff = false;
            }
        }
    }
    if(key == GLFW_KEY_P){
        if(action == GLFW_PRESS){
            if(camera.if_paralax == false){
                camera.if_paralax = true;
            }else{
                camera.if_paralax = false;
            }
        }
    }
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos){
    if(camera.first_mouse){
        camera.last_x = x_pos;
        camera.last_y = y_pos;
        camera.first_mouse = false;
    }
    GLfloat x_offset = x_pos - camera.last_x;
    GLfloat y_offset = camera.last_y - y_pos; 
    camera.last_x = x_pos;
    camera.last_y = y_pos;
/*****************************************************/
    GLfloat mouse_sensitivity = 0.05;
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;
    camera.yaw   += x_offset;
    camera.pitch += y_offset;
    if(camera.pitch > 89.0f){
        camera.pitch = 89.0f;
    }else   if(camera.pitch < -89.0f){
        camera.pitch = -89.0f;
    }
/***************************************************/
}
int my_camera_step(){
    GLfloat new_time = glfwGetTime();
    camera.delta_time = new_time - camera.last_time;
    camera.last_time = new_time;
    GLfloat speed = 1.3f * camera.delta_time;

    glm::vec3 dirfront;
    dirfront.x = cos(camera.pitch * 0.017453f) * cos(camera.yaw * 0.017453f);
    dirfront.y = sin(camera.pitch * 0.017453f);
    dirfront.z = cos(camera.pitch * 0.017453f) * sin(camera.yaw * 0.017453f);
    camera.camera_front = glm::normalize(dirfront);

    if(camera.keystate[1] == true)
        camera.camera_pos += speed * camera.camera_front;
    if(camera.keystate[3] == true)
        camera.camera_pos -= speed * camera.camera_front;
    if(camera.keystate[0] == true)
        camera.camera_pos -= glm::normalize(glm::cross(camera.camera_front, camera.camera_up)) * speed;
    if(camera.keystate[2] == true)
        camera.camera_pos += glm::normalize(glm::cross(camera.camera_front, camera.camera_up)) * speed;
    return 0;
}


