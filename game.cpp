#include "game.h"
#include "Camera.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");

	simpleShader->Bind();
	mat4 projection = mat4::Perspective(PI / 180 * 45, static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
	                                    0.1f, 100.0f);
	simpleShader->SetMat4x4("projection", projection);

	simpleShader->SetInt("wallTexture", 0); // or with shader class
	simpleShader->SetInt("faceTexture", 1); // or with shader class

	simpleShader->Unbind();


	triangle.Init();
	camera = new Camera();
	camera->Init();
	camera->RotateMouse(mousePos);
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------


float mixing = .2f;
float3 cubePositions[] = {
	float3(0.0f, 0.0f, 0.0f),
	float3(2.0f, 5.0f, -15.0f),
	float3(-1.5f, -2.2f, -2.5f),
	float3(-3.8f, -2.0f, -12.3f),
	float3(2.4f, -0.4f, -3.5f),
	float3(-1.7f, 3.0f, -7.5f),
	float3(1.3f, -2.0f, -2.5f),
	float3(1.5f, 2.0f, -2.5f),
	float3(1.5f, 0.2f, -1.5f),
	float3(-1.3f, 1.0f, -1.5f)
};
float3 position;

void Game::HandleInput(float deltaTime)
{
	if (keystate[XK_Escape])
		exit(0);


	if (keystate[XK_Left])
		camera->MoveX(1);
	if (keystate[XK_Right])
		camera->MoveX(-1);

	if (keystate[XK_Down])
		camera->MoveZ(-1);

	if (keystate[XK_Up])
		camera->MoveZ(1);
	camera->RotateMouse(mousePos);
}

void Game::Tick(float deltaTime)
{
	HandleInput(deltaTime);

	simpleShader->Bind();


	//simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	simpleShader->SetFloat("mixing", mixing);


	camera->Update(deltaTime);

	simpleShader->SetMat4x4("view", camera->LookAt());

	//camera->SetViewMatrix(simpleShader);
	for (unsigned int i = 0; i < 10; i++)
	{
		mat4 model = mat4::Translate(cubePositions[i]);
		float angle = 20.0f * i;
		float3 dir(1.0f, 0.3f, 0.5f);
		dir = normalize(dir);
		model = model * mat4::Rotate(dir.x, dir.y, dir.z, angle * PI / 180);
		simpleShader->SetMat4x4("model", model);
		triangle.Draw();
	}

	simpleShader->Unbind();
}
