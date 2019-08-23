#include "context.h"
#include "render.h"
#include <string>
#include <iostream>


int main()
{


	context game;
	game.init(640, 480, "Source3");

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(errorCallback, 0);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
#endif

	vertex triVerts[] =
	{
		{ { -.5, -.5, 0, 1 } },
		{ {  .5, -.5, 0, 1 } },
		{ {   0,  .5, 0, 1 } }
	};
	
	unsigned int triIndices[] = { 0, 1, 2 };

	geometry triangle = makeGeometry(triVerts, 3, triIndices, 3);

	const char * basicVert =
		"#version 410\n"
		"layout (location = 0) in vec4 position;\n"
		"void main() { gl_Position = position; }";

	std::string vertShadFilepath = "C:/Users/s189062/source/repos/aierenderer/shaderfiles/vertShader.txt";
	std::string fileVert = loadVertShad(vertShadFilepath);

	const char * basicFrag =
		"#version 330\n"
		"out vec4 vertColor;\n"
		"void main() { vertColor = vec4(1.0, 0.0, 0.0, 1.0); }";

	shader basicShad = makeShader(fileVert.c_str(), basicFrag);

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		draw(basicShad, triangle);
	}

	game.term();

	return 0;
}