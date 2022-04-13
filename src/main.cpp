#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

int main(){
    cout << "Hello, world!\n";

    GLFWwindow* window;
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
		printf("Shader compilation error!\n");
		vector<char> errors(infoLogLength+1);
		glGetShaderInfoLog(compShaderID, infoLogLength, NULL, &errors[0]);
		printf("%s\n", &errors[0]);
	}

	// Link program
	GLuint programID = glCreateProgram();
	glAttachShader(programID, compShaderID);
	glLinkProgram(programID);

	// glDetachShader(programID, compShaderID);
	// glDeleteShader(compShaderID);

	// Trying buffers
	float vals1[64];
	for(int i=0; i<64; i++){
		vals1[i] = 0;
	}

	GLuint ssbo = 0;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float[64]), &vals1, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	glUseProgram(programID);
	glDispatchCompute(16,1,1);

	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	float* returnedData = (float*) glMapBuffer(GL_SHADER_STORAGE_BUFFER,  GL_READ_ONLY);
	for(int i=0; i<64; i++){
		cout << returnedData[i] << " ";
	}
	cout << endl;

	glfwTerminate();

	return 0;
}
