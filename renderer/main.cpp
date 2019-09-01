#include "context.h"
#include "render.h"
#include "fileLoader.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

int main()
{
	context game;
	game.init(640, 480, "Source3");

	#pragma region Triangle
	//Triangle
	vertex triVerts[] =
	{
		{ {-0.5f, -0.5f, 0, 1}, { 0, 0, 1, 0 }, {0.f, 0.f}, { 0, 1, 0, 1 } },
		{ {0.5f,  -0.5f, 0, 1}, { 0, 0, 1, 0 }, {1.f, 0.f}, { 1, 0, 0, 1 } },
		{ {0,      0.5f, 0, 1}, { 0, 0, 1, 0 }, {.5f, 1.f}, { 0, 0, 1, 1 } }
	};
	unsigned int triIndices[] = { 0, 1, 2 };
	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);
	#pragma endregion
	#pragma region Quad
	// Quad
	vertex quadVerts[] =
	{	// pos, color, uvs, normals
		{ {  .5, -.5, 0, 1 }, {1, 0, 0, 1}, {0.f, 0.f}, {0, 0, 1, 0} },
		{ {  .5,  .5, 0, 1 }, {0, 1, 0, 1}, {0.f, 1.f}, {0, 0, 1, 0} },
		{ { -.5, -.5, 0, 1 }, {0, 0, 1, 1}, {1.f, 0.f}, {0, 0, 1, 0} },
		{ { -.5,  .5, 0, 1 }, {0, 0, 1, 1}, {1.f, 1.f}, {0, 0, 1, 0} }
	};
	unsigned int quadIndices[] = { 0, 1, 2, 1, 3, 2 };
	geometry quad = { makeGeometry(quadVerts, 4, quadIndices, 6) };
	#pragma endregion

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> meshes = fileLoadObj("Geometry/tri.obj", attrib);

	geometry modelGeo = fileLoadObj("Geometry/tri.obj");

	#pragma region Lame model building
	// Building the model
	// vertices

	int vertexCount = attrib.vertices.size() / 3;
	std::vector<vertex> modelVerts(vertexCount);
	for (size_t i = 0; i < vertexCount; i++)
	{
		modelVerts[i].pos.x = attrib.vertices[3 * i + 0];
		modelVerts[i].pos.y = attrib.vertices[3 * i + 1];
		modelVerts[i].pos.z = attrib.vertices[3 * i + 2];
		modelVerts[i].pos.w = 1;
	}

	// Color
	for (size_t i = 0; i < vertexCount; i++)
	{
		modelVerts[i].color = { 1, 0, 0, 1 };
	}

	// normals
	int normalCount = attrib.normals.size() / 3;
	for (size_t i = 0; i < normalCount; i++)
	{
		modelVerts[i].norm.x = attrib.normals[3 * i + 0];
		modelVerts[i].norm.y = attrib.normals[3 * i + 1];
		modelVerts[i].norm.z = attrib.normals[3 * i + 2];
		modelVerts[i].norm.w = 0;
	}

	// UVs
	int UVCount = attrib.texcoords.size() / 2;
	for (size_t i = 0; i < UVCount; i++)
	{
		//TODO: Figure out why there are so many UVs
		/*modelVerts[i].uv.x = attrib.texcoords[2 * i + 0];
		modelVerts[i].uv.y = attrib.texcoords[2 * i + 1];*/
	}

	// Indicies
	int indicesCount = meshes[0].mesh.indices.size();
	std::vector<unsigned int> modelIndices(indicesCount);
	for (size_t i = 0; i < meshes[0].mesh.indices.size(); i++)
	{
		modelIndices[i] = meshes[0].mesh.indices[i].vertex_index;
	}

	geometry modelTriangles[] = {
		{makeGeometry(modelVerts.data(), modelVerts.size(), modelIndices.data(), 3) }
	};
#pragma endregion

	#pragma region Shaders
	// Shader loading
	shader basicShad = makeShader(fileLoadShader("Shaders/basic.vert").c_str(), fileLoadShader("Shaders/basic.frag").c_str());
	shader camShad = makeShader(fileLoadShader("Shaders/cam.vert").c_str(), fileLoadShader("Shaders/cam.frag").c_str());
	shader colorShad = makeShader(fileLoadShader("Shaders/color.vert").c_str(), fileLoadShader("Shaders/color.frag").c_str());
	shader lightShad = makeShader(fileLoadShader("Shaders/light.vert").c_str(), fileLoadShader("Shaders/light.frag").c_str());
	shader uvShad = makeShader(fileLoadShader("Shaders/uv.vert").c_str(), fileLoadShader("Shaders/uv.frag").c_str());

	#pragma endregion

	// set up camera
	glm::mat4 camProj = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 100.0f);
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 triModel = glm::identity<glm::mat4>();

	// load textures
	texture camiTex = loadTexture("Textures/quadTexture.png");

	// set up light
	light sun;
	sun.direction = glm::vec3{ -1, 0, 0};
	sun.color = glm::vec4{0, 0 , 1, 1};

	// set shader uniforms
	setUniform(camShad, 0, camProj);
	setUniform(camShad, 1, camView);
	setUniform(camShad, 2, triModel);

	setUniform(lightShad, 0, camProj);
	setUniform(lightShad, 1, camView);
	setUniform(lightShad, 2, triModel);
	setUniform(lightShad, 3, camiTex, 0);
	setUniform(lightShad, 4, sun.direction);
	//setUniform(lightShad, 5, sun.color);

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		assert(glGetError() == GL_NO_ERROR);

		triModel = glm::rotate(triModel, glm::radians(1.f), glm::vec3(0, 1, 0));
		setUniform(lightShad, 2, triModel);

		draw(lightShad, quad);
		
	}

	freeGeometry(quad);
	freeGeometry(modelGeo);
	freeShader(basicShad);
	freeShader(colorShad);
	freeShader(camShad);
	freeShader(lightShad);
	freeShader(uvShad);
	freeTexture(camiTex);
	game.term();

	return 0;
}