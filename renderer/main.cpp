#include "context.h"
#include "render.h"
#include "loader.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

int main()
{


	context game;
	game.init(640, 480, "Source3");

	std::string meshFilePath = "C:/Users/s189062/source/repos/aierenderer/meshes/cube.obj";
	std::vector<tinyobj::shape_t> meshes = fileLoadMesh(meshFilePath);

	// Triangle, CCW
	vertex triVerts[] =
	{
		{ {  .5, -.5, 0, 1 }, {1, 0, 0, 1}, {0.f, 0.f} },
		{ {  .5,  .5, 0, 1 }, {0, 1, 0, 1}, {0.f, 1.f} },
		{ { -.5,  -.5, 0, 1 }, {0, 0, 1, 1}, {1.f, 0.f} },
		{ { -.5, .5, 0, 1 }, {0, 0, 1, 1}, {1.f, 1.f} }
	};
	
	unsigned int triIndices[2][3] = { { 0, 1, 2 }, { 1, 3, 2 } };

	geometry triangles[] = { makeGeometry(triVerts, 4, triIndices[0], 3),
							makeGeometry(triVerts, 4, triIndices[1], 3) };

	std::string vertShadFilePath = "C:/Users/s189062/source/repos/aierenderer/shaderfiles/camUVVert.txt";
	std::string fileVert = fileLoadShader(vertShadFilePath);

	std::string fragShadFilePath = "C:/Users/s189062/source/repos/aierenderer/shaderfiles/camUVFrag.txt";
	std::string fileFrag = fileLoadShader(fragShadFilePath);

	shader basicShad = makeShader(fileVert.c_str(), fileFrag.c_str());

	glm::mat4 triModel = glm::identity<glm::mat4>();

	glm::mat4 camProj = glm::perspective(glm::radians(45.f), 640.f / 480.f, 0.1f, 100.0f);
	glm::mat4 camView = glm::lookAt(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	texture triTex = loadTexture("C:/Users/s189062/source/repos/aierenderer/textures/quadTexture.png");

	setUniform(basicShad, 0, camProj);
	setUniform(basicShad, 1, camView);
	setUniform(basicShad, 3, triTex, 0);

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		assert(glGetError() == GL_NO_ERROR);

		//triModel = glm::rotate(triModel, glm::radians(1.f), glm::vec3(0, 1, 0));
		
		setUniform(basicShad, 2, triModel);

		draw(basicShad, triangles[0]);
		draw(basicShad, triangles[1]);
	}

	game.term();

	return 0;
}