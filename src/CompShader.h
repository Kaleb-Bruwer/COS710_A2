#pragma once

#include <GLFW/glfw3.h>

class CompShader{
private:
    GLFWwindow* window = 0;

    void initOpenGl(); //Probably shouldn't run this more than once

public:
    CompShader();

};
