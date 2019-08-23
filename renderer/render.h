#pragma once

#include "glew/GL/glew.h"
#include "glm/glm.hpp"
#include <string>

struct vertex
{
	glm::vec4 pos;
};

struct geometry
{
	GLuint vao, vbo, ibo;	// buffers
	GLuint size;			// index count
};

struct shader
{
	GLuint program;
};

geometry makeGeometry(vertex * verts, size_t vertCount,
						unsigned * indicies, size_t indexCount);

void freeGeometry(geometry &geo);

shader makeShader(const char * vertSource,
					const char * fragSource);
void freeShader(shader &shad);

void draw(const shader &shad, const geometry &geo);

void GLAPIENTRY errorCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar *message, const void *userParam);

std::string loadVertShad(std::string filepath);