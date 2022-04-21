#include "CompShader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>

#include "DataLoader.h"


using namespace std;

CompShader::CompShader(string filename){
    initialize(filename);
}

void CompShader::initialize(string filename){
    if(initOpenGl() && compileShader(filename))
        usable = true;
}


CompShader::~CompShader(){
    glfwTerminate();
}


bool CompShader::initOpenGl(){
    // Initialise GLFW
    if(!glfwInit()){
        cout << "Couldn't initialize glfw\n";
        return false;
    }

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1, 1, "Assignment 2", NULL, NULL);
    if( window == NULL ){
        cout << "Failed to open window, unfortunately this is needed even for compute shaders\n";
        getchar();
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        cout << "Failed to initialize GLEW\n";
        getchar();
        glfwTerminate();
        return false;
    }
    return true;
}

bool CompShader::compileShader(string filename){
    string compShaderSource;
    ifstream compShaderStream(filename, ios::in);
    if(compShaderStream.is_open()){
        stringstream sstr;
        sstr << compShaderStream.rdbuf();
        compShaderSource = sstr.str();
        compShaderStream.close();
    }else{
        cout << "Couldn't load shader source code\n";
        return false;
    }

    const char* constSrc = compShaderSource.c_str();

	GLuint compShaderID = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compShaderID, 1, &constSrc, NULL);
	glCompileShader(compShaderID);

	// Check for comp shader errors
	GLint result = GL_FALSE;
	int infoLogLength;

	glGetShaderiv(compShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(compShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
        cout << "Shader compilation error!" << endl;
		vector<char> errors(infoLogLength+1);
		glGetShaderInfoLog(compShaderID, infoLogLength, NULL, &errors[0]);
        cout << &errors[0] << endl;
        return false;
	}

	// Link program
	programID = glCreateProgram();
	glAttachShader(programID, compShaderID);
	glLinkProgram(programID);

    glDetachShader(programID, compShaderID);
	glDeleteShader(compShaderID);

    return true;
}

void CompShader::loadData(void* gpuData, unsigned int len){
    GL_data = 0;
    glGenBuffers(1, &GL_data);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_data);
    glBufferData(GL_SHADER_STORAGE_BUFFER, len, gpuData, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, GL_data);

    cout << "Data has been placed on the gpu\n";
}

void CompShader::loadTrees(GPUNode* trees, unsigned int len,
        vector<unsigned int> &starts){
    GL_trees = 0;
    glGenBuffers(1, &GL_trees);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_trees);
    glBufferData(GL_SHADER_STORAGE_BUFFER, len*sizeof(GPUNode), trees, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, GL_trees);

    GL_starts = 0;
    glGenBuffers(1, &GL_starts);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_starts);
    glBufferData(GL_SHADER_STORAGE_BUFFER, starts.size()*sizeof(unsigned int), &starts[0], GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, GL_starts);

    cout << "Trees have been placed on the gpu\n";
}

/*
TODO: execute more than GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS trees by
doing multiple dispatches (will require changes to GL_starts)
*/
void CompShader::execShader(float* fitness, int nTrees){
    if(!usable){
        cout << "Attempted to execute an unusable shader\n";
        return;
    }
    if(!fitness){
        cout << "Attempted to execute shader with no fitness destination\n";
        return;
    }

    // GL_fitness will actually be used for output, but is being initialized as normal
    GL_fitness = 0;
    glGenBuffers(1, &GL_fitness);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_fitness);
    glBufferData(GL_SHADER_STORAGE_BUFFER, nTrees * sizeof(float), fitness, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, GL_fitness);


    glUseProgram(programID);
    glDispatchCompute(nTrees,1,1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, GL_fitness);
    float* fitRet = (float*) glMapBuffer(GL_SHADER_STORAGE_BUFFER,  GL_READ_ONLY);
    memcpy(fitness, fitRet, nTrees*sizeof(float));


}
