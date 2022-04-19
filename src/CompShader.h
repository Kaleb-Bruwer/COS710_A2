#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class CompShader{
private:
    GLFWwindow* window = 0;
    GLuint programID;
    bool usable = false;

    GLuint GL_data;

    bool initOpenGl(); //Probably shouldn't run this more than once
    bool compileShader(std::string filename);

public:
    CompShader(){};
    CompShader(std::string filename);
    void initialize(std::string filename = "../src/compshader.glsl");

    void loadData(void* data, unsigned int len);

    ~CompShader();

    void execShader();

};
