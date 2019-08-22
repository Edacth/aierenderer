#include "context.h"
#include "render.h"

int main()
{
	context game;
	game.init(640, 480, "Source3");

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

	const char * basicFrag =
		"#version 330\n"
		"out vec4 vertColor;\n"
		"void main() { vertColor = vec4(1.0, 0.0, 0.0, 1.0); }";

	shader basicShad = makeShader(basicVert, basicFrag);

	while (!game.shouldClose())
	{
		game.tick();
		game.clear();

		draw(basicShad, triangle);
	}

	game.term();

	return 0;
}