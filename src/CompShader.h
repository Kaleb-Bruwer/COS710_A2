#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <Population/GPUNode.h>

class CompShader{
private:
    GLFWwindow* window = 0;
    GLuint programID;
    bool usable = false;

    GLuint GL_data;
    GLuint GL_trees;
    GLuint GL_starts;
    GLuint GL_fitness;

    bool initOpenGl(); //Probably shouldn't run this more than once
    bool compileShader(std::string filename);

public:
    CompShader(){};
    CompShader(std::string filename);
    void initialize(std::string filename = "../src/compshader.glsl");

    void loadData(void* data, unsigned int len);
    void loadTrees(GPUNode* trees, unsigned int len,
            std::vector<unsigned int> &starts);

    ~CompShader();

    void execShader(float* fitness, int nTrees, int numInputs);

};
