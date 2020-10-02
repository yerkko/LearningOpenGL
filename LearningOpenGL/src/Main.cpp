#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <memory>
#include <sstream>
#include <fstream>
#include <string>
#include "common/ShaderParser.h"



static unsigned int CompileShader(unsigned int type, const std::string& source) {

	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int compiled;

	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {

		int len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		char* log = new char[len];

		glGetShaderInfoLog(id, len, &len, log);

		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader" << std::endl;

		glDeleteShader(id);
		delete[](log);
		return 0;

	}

	return id;


}


static int CreateShader(const std::string& vertexShader, const  std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return 1;


}


int main(void)
{
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Learning OpenGL", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error?" << std::endl;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


	float positions[8] = {
	 -0.5f, -0.5f,  // 0 
	  0.5f, -0.5f ,  // 1
	  0.5f, 0.5f ,  // 2
	 -0.5f, 0.5f   // 3
	}; 

	unsigned int indices[] = 
	
	
	{ 0, 1, 2,
	  2, 3, 0 };

	/*

	x ----- x 0.5, 0.5 (2)
	|		|
	|		|
	|		|
	|		|
	x ----- x 0.5, -0.5 (1) 
	
	
	
	*/


	// vertex buffer

	unsigned int vertexBuffer;
	glGenBuffers(1, &vertexBuffer);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(positions[0]), 0);









	//index buffer

	unsigned int indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	//shader setup

	
	const std::string vs = parseShader("res/sh_basic/sh_vertex.glsl");
	const std::string fs = parseShader("res/sh_basic/sh_fragment.glsl");

	unsigned int shader = CreateShader(vs, fs);

	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}