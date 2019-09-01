#include "context.h"
#include "render.h"
#include "fileLoader.h"

#include <string>
#include <iostream>
#include <vector>
#include <cassert>

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glfw/glfw3.h"

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

	geometry modelGeo = fileLoadObj("Geometry/soulspear.obj");

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
	glm::vec3 cameraMovement{ 0, 0, 0 };
	float cameraSpeed = 0.05;
	glm::vec3 modelRotation{ 0, 0, 0 };
	float rotationSpeed = 1;

	// load textures
	texture camiTex = loadTexture("Textures/quadTexture.png");
	texture uvCheckerTex = loadTexture("Textures/uvchecker.jpg");
	texture soulSpearTex = loadTexture("Textures/soulspear_diffuse.tga");

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
	setUniform(lightShad, 3, soulSpearTex, 0);
	setUniform(lightShad, 4, sun.direction);
	//setUniform(lightShad, 5, glm::vec4{1, 1, 1, 1});

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		assert(glGetError() == GL_NO_ERROR);

		
		if (glfwGetKey(game.getWindow(), GLFW_KEY_UP) == GLFW_PRESS) { cameraMovement.y += cameraSpeed; }
		if (glfwGetKey(game.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) { cameraMovement.y += -cameraSpeed; }
		if (glfwGetKey(game.getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) { cameraMovement.x += cameraSpeed; }
		if (glfwGetKey(game.getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) { cameraMovement.x += -cameraSpeed; }

		if (cameraMovement != glm::vec3{0, 0 ,0})
		{
			triModel = glm::translate(triModel, cameraMovement);
			cameraMovement = glm::vec3{ 0, 0 ,0 };
		}

		if (glfwGetKey(game.getWindow(), GLFW_KEY_A) == GLFW_PRESS) { modelRotation.y += 1; }
		if (glfwGetKey(game.getWindow(), GLFW_KEY_D) == GLFW_PRESS) { modelRotation.y += -1; }
		if (glfwGetKey(game.getWindow(), GLFW_KEY_W) == GLFW_PRESS) { modelRotation.x += 1; }
		if (glfwGetKey(game.getWindow(), GLFW_KEY_S) == GLFW_PRESS) { modelRotation.x += -1; }

		if (modelRotation != glm::vec3{ 0, 0 ,0 })
		{
			triModel = glm::rotate(triModel, glm::radians(rotationSpeed), modelRotation);
			modelRotation = glm::vec3{ 0, 0 ,0 };
		}
		
		
		setUniform(lightShad, 2, triModel);

		draw(lightShad, modelGeo);
		
	}

	freeGeometry(quad);
	freeGeometry(modelGeo);
	freeShader(basicShad);
	freeShader(colorShad);
	freeShader(camShad);
	freeShader(lightShad);
	freeShader(uvShad);
	freeTexture(camiTex);
	freeTexture(uvCheckerTex);
	game.term();

	return 0;
}