#include "game.h"

#include "Timer.h"

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
float mixing = .2f;

void Game::Tick(float deltaTime)
{
	static Timer timer;
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


	mat4 rotate = mat4::RotateZ(timer.elapsed());
	mat4 scale = mat4::Scale(sinf(timer.elapsed()));
	mat4 translate1 = mat4::Translate(0.5f, -0.5f, 0);
	mat4 translate2 = mat4::Translate(-.5f, 0.5, 0);

	mat4 transform = translate1 * rotate;

	simpleShader->Bind();
	//simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	simpleShader->SetFloat("mixing", mixing);
	simpleShader->SetMat4x4("transform", transform);

	triangle.Draw();
	transform = translate2 * scale;

	simpleShader->SetMat4x4("transform", transform);

	triangle.Draw();

	simpleShader->Unbind();
}
