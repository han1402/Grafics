#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>

std::string read_sh_code(const char * code_path);

GLuint compl_shader(const char * vertex_path, const char * fragment_path)
{
    GLuint program;
/***********************************************************************/
    std::string vertex_code = read_sh_code(vertex_path);
    std::string fragment_code = read_sh_code(fragment_path);
    const GLchar * glchar_vertex_code = vertex_code.c_str();
    const GLchar * glchar_fragment_code = fragment_code.c_str();
    GLuint vertex_sh, fragment_sh;
    GLint correct;
    GLchar ShaderinfoLog[512];
    vertex_sh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_sh, 1, &glchar_vertex_code, NULL);
    glCompileShader(vertex_sh);
    glGetShaderiv(vertex_sh, GL_COMPILE_STATUS, &correct);
    if(!correct)
    {
        glGetShaderInfoLog(vertex_sh, 512, NULL, ShaderinfoLog);
        std::cout << "error SH VERTEX COMPILATION FAILED\n" << ShaderinfoLog << std::endl;
    }
    fragment_sh = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragment_sh, 1, &glchar_fragment_code, NULL);
    glCompileShader(fragment_sh);
    glGetShaderiv(fragment_sh, GL_COMPILE_STATUS, &correct);
    if(!correct)
    {
        glGetShaderInfoLog(fragment_sh, 512, NULL, ShaderinfoLog);
        std::cout << "error SH FRAGMENT COMPILATION FAILED\n" << ShaderinfoLog << std::endl;
    }
/**********************************************************************/
    program = glCreateProgram();
    glAttachShader(program, vertex_sh);
    glAttachShader(program, fragment_sh);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &correct);
    if(!correct)
    {
    glGetProgramInfoLog(program, 512, NULL, ShaderinfoLog);
        std::cout << "error LINK SH PROGRAM" << ShaderinfoLog << std::endl;
    }
/**********************************************************************/
    glDeleteShader(vertex_sh);
    glDeleteShader(fragment_sh);
    return program;
}

std::string read_sh_code(const char * code_path)
{
    if(code_path == NULL)
    {
        std::cout << "error no path" << std::endl;
        return "error";
    }
    std::ifstream file;
    file.open(code_path);
    if(file.is_open() == 0)
    {
        std::cout << "error open " << code_path << std::endl;
        return "error";
    }
/*********************************************************************/
    std::string tmpstr;
    std::string shcode = "";
    while(file.eof() != true)
    {
        getline(file, tmpstr);
        shcode += tmpstr;
        shcode += " \n";
    }
/*********************************************************************/
    file.close();
    return shcode;
}
#endif
