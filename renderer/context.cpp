// the file we're implementing
#include "context.h"

// system headers
#include <iostream>

// library headers
#include "glew/GL/glew.h"
#include "glfw/glfw3.h"

// opther projects headers


GLDEBUGPROC errorDisplay()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << err << std::endl;
	}
	return GLDEBUGPROC();
}

// Error callback called by OpenGL whenever a problem occurs (vendor-dependent)
void GLAPIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message,
	const void *userParam)
{
	//std::cerr << message << std::endl;

	std::cout << "ID: " << id << std::endl;
	std::cout << "Message: " << message << std::endl;
}

bool context::init(int width, int height, const char * title)
{
	glfwInit();

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glewInit();

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif

	std::cout << "OpenGL Version:" << (const char *)glGetString(GL_VERSION) << "\n";
	std::cout << "Renderer:" << (const char *)glGetString(GL_RENDERER) << "\n";
	std::cout << "Vendor:" << (const char *)glGetString(GL_VENDOR) << "\n";
	std::cout << "GLSL:" << (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

	return true;
}

void context::tick()
{
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void context::term()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	window = nullptr;
}

bool context::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

void context::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

