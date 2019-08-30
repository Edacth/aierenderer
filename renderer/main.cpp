#include "context.h"
#include "render.h"
#include "loader.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

int main()
{
	// TODO: Try moving the geo construction into the fileLoadMesh function. Rename it to fileLoadGeo?
	context game;
	game.init(640, 480, "Source3");

	std::string objFilePath = "C:/Users/s189062/source/repos/aierenderer/meshes/cube.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> meshes = fileLoadMesh(objFilePath, attrib);

	// Triangle, CCW
	vertex triVerts[] =
	{	// pos, color, uvs, normals
		{ {  .5, -.5, 0, 1 }, {1, 0, 0, 1}, {0.f, 0.f}, {0, 0, 1, 0} },
		{ {  .5,  .5, 0, 1 }, {0, 1, 0, 1}, {0.f, 1.f}, {0, 0, 1, 0} },
		{ { -.5, -.5, 0, 1 }, {0, 0, 1, 1}, {1.f, 0.f}, {0, 0, 1, 0} },
		{ { -.5,  .5, 0, 1 }, {0, 0, 1, 1}, {1.f, 1.f}, {0, 0, 1, 0} }
	};
	unsigned int triIndices[2][3] = { { 0, 1, 2 }, { 1, 3, 2 } };
	geometry triangles[] = { makeGeometry(triVerts, 4, triIndices[0], 3),
							makeGeometry(triVerts, 4, triIndices[1], 3) };
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

	std::string vertShadFilePath = "C:/Users/s189062/source/repos/aierenderer/shaderfiles/camVert.txt";
	std::string fileVert = fileLoadShader(vertShadFilePath);

	std::string fragShadFilePath = "C:/Users/s189062/source/repos/aierenderer/shaderfiles/camFrag.txt";
	std::string fileFrag = fileLoadShader(fragShadFilePath);

	shader basicShad = makeShader(fileVert.c_str(), fileFrag.c_str());

	glm::mat4 triModel = glm::identity<glm::mat4>();

	glm::mat4 camProj = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 100.0f);
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	texture triTex = loadTexture("C:/Users/s189062/source/repos/aierenderer/textures/quadTexture.png");

	light sun;
	sun.direction = glm::vec4{ -1, 0, 0, 1 };

	setUniform(basicShad, 0, camProj);
	setUniform(basicShad, 1, camView);
	//setUniform(basicShad, 3, triTex, 0);
	//setUniform(basicShad, 4, sun.direction);

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		assert(glGetError() == GL_NO_ERROR);

		triModel = glm::rotate(triModel, glm::radians(1.f), glm::vec3(0, 1, 0));
		
		setUniform(basicShad, 2, triModel);

		//draw(basicShad, triangles[0]);
		//draw(basicShad, triangles[1]);
		draw(basicShad, modelTriangles[0]);
	}

	game.term();

	return 0;
}