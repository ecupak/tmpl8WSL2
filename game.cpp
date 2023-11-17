#include "game.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");

	simpleShader->Bind();

	simpleShader->SetInt("wallTexture", 0); // or with shader class
	simpleShader->SetInt("faceTexture", 1); // or with shader class

	simpleShader->Unbind();


	triangle.Init();
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------

float3 position = 0;
float mixing = 0;

void Game::Tick(float deltaTime)
{
	if (keystate[XK_Escape])
		exit(0);
	if (keystate[XK_Left])
		position.x += -1 * deltaTime;
	if (keystate[XK_Right])
		position.x += 1 * deltaTime;
	if (keystate[XK_Down])
		mixing += deltaTime;
	if (keystate[XK_Up])
		mixing -= deltaTime;


	simpleShader->Bind();
	simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	simpleShader->SetFloat("mixing", mixing);

	triangle.Draw();
	simpleShader->Unbind();
}
