#pragma once

#include "glew/GL/glew.h"
#include "glm/glm.hpp"
#include <string>

struct vertex
{
	glm::vec4 pos;
	glm::vec4 color;
	glm::vec2 uv;
	glm::vec4 norm;
};

struct mesh
{
	int vertCount;
	int indexCount;
	vertex* verts;
	unsigned int* indices;

	mesh(vertex* _verts, int vertCount, unsigned int* _indices, int indexCount)
	{
		for (size_t i = 0; i < vertCount; i++)
		{
			verts[i] = _verts[i];
		}
		for (size_t i = 0; i < indexCount; i++)
		{
			indices[i] = _indices[i];
		}
		
	}
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

struct texture
{
	GLuint handle;
	unsigned int width, height, channels;
};

struct light
{
	glm::vec3 direction;
	glm::vec4 color;
};

geometry makeGeometry(vertex * verts, size_t vertCount,
						unsigned * indicies, size_t indexCount);

void freeGeometry(geometry &geo);

shader makeShader(const char * vertSource,
					const char * fragSource);
void freeShader(shader &shad);

void draw(const shader &shad, const geometry &geo);

void setUniform(const shader &shad, GLuint location, const glm::vec3 & value);

void setUniform(const shader &shad, GLuint location, const glm::mat4 & value);

void setUniform(const shader &shad, GLuint location, const texture &value, int textureSlot);

texture makeUniform(const shader &shad, GLuint location, const glm::mat4 & value);

void freeTexture(texture &tex);

texture loadTexture(const char *imagePath);
