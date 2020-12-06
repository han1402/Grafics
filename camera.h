#ifndef CAMERA_H
#define CAMERA_H

struct myCamera{
    GLfloat delta_time;
    GLfloat last_time;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat last_x;
    GLfloat last_y;
    bool first_mouse;
    float screen_width, screen_height;
    bool peff;
    bool keystate[4]; // for key    A W D S
    glm::vec3 camera_pos;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
};



#endif
