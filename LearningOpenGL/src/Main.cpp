#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <memory>

#define uint unsigned int

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
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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


	/*float positions[6] = {
	-0.5f, -0.5f,
	0.0f, 0.5f,
	0.5f, -0.5f,
	};*/



	typedef struct v2 {
		float x;
		float y;

	}v2f;

	v2f positions[3] = {

		v2f{-0.5f,-0.5f},
		v2f{0.0f,0.5f},
		v2f{0.5f,-0.5f}

	};


	unsigned int buffer;

	glGenBuffers(1, &buffer);


	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(positions[0]), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(positions[0]), 0);

	const std::string vs = R"glsl(
#version 330 core

layout(location = 0) in vec4 position;

void main(){
   gl_Position = position;
}
)glsl";


	const std::string fs = R"glsl(
#version 330 core

layout(location = 0) out vec4 color;

void main(){
   color = vec4(0.0,1.0,0.0,1.0);
}
)glsl";

	unsigned int shader = CreateShader(vs, fs);

	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}