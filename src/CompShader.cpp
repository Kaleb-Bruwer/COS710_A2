#include "CompShader.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

CompShader::CompShader(){

}

void CompShader::initOpenGl(){
    // Initialise GLFW
    if(!glfwInit()){
        cout << "Couldn't initialize glfw\n";
        return -1;
    }

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1, 1, "Assignment 2", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Compile comp shader
    string compShaderSource;
    ifstream compShaderStream("../src/compshader.glsl", ios::in);
    if(compShaderStream.is_open()){
        stringstream sstr;
        sstr << compShaderStream.rdbuf();
        compShaderSource = sstr.str();
        compShaderStream.close();
    }else{
        cout << "Couldn't load shader source code\n";
        return -1;
    }
}
