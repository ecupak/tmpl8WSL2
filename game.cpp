#include "template.h"
#include "Triangle.h"
// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	triangle.Init();
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------


void Game::Tick(float deltaTime)
{
	if (keystate[XK_Escape])
		exit(0);


	triangle.Draw();
}
