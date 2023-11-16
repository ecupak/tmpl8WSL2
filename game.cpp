#include "template.h"
#include "Triangle.h"
// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	triangle.Init();

	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------

float3 position = 0;

void Game::Tick(float deltaTime)
{
	if (keystate[XK_Escape])
		exit(0);
	if (keystate[XK_Left])
		position.x += -1 * deltaTime;
	if (keystate[XK_Right])
		position.x += 1 * deltaTime;

	simpleShader->Bind();
	simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	triangle.Draw();
	simpleShader->Unbind();
}
