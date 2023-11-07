#include "template.h"
Surface test("assets/spec.jpg");

int xpos = 50, ypos = 50;

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	xpos = 20;
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------
void Game::Tick(float deltaTime)
{
	//add the include
	if (keystate[XK_Escape])
		exit(0);
	if (keystate[XK_Left])
		xpos--;

	if (keystate[XK_Right])
		xpos++;
	if (keystate[XK_Up])
		ypos--;
	if (keystate[XK_Down])
		ypos++;
	screen->Clear(0);
	screen->Print("hello world", 2, 2, 0xffffff);
	test.CopyTo(screen, xpos, ypos);
	if (mousePos.x > 0 && mousePos.y > 0 &&
		mousePos.x < screen->width && mousePos.y < screen->height)
		screen->pixels[mousePos.x + mousePos.y * screen->width] = 0xffffffff;
	//printf("hello world'\n");
}
