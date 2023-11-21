#include "game.h"

#include <iostream>

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


	simpleShader->SetInt("wallTexture", 0); // or with shader class
	simpleShader->SetInt("faceTexture", 1); // or with shader class

	simpleShader->Unbind();


	triangle.Init();
	camera = new Camera();
	camera->Init();
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
float fov = 45;
float yOffset = 0;

int2 rotateCam = 0;
float2 moveCam = 0;


void Game::Tick(float deltaTime)
{
	camera->RotateMouse(rotateCam);
	camera->MoveX(moveCam.x);
	camera->MoveZ(moveCam.y);
	fov -= yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
	yOffset = 0;

	simpleShader->Bind();


	//simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	simpleShader->SetFloat("mixing", mixing);


	camera->Update(deltaTime);

	mat4 projection = mat4::Perspective(fov * TO_RADIANS, static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
	                                    0.1f, 100.0f);
	simpleShader->SetMat4x4("projection", projection);
	simpleShader->SetMat4x4("view", camera->LookAt());

	//camera->SetViewMatrix(simpleShader);
	for (unsigned int i = 0; i < 10; i++)
	{
		mat4 model = mat4::Translate(cubePositions[i]);
		float angle = 20.0f * i;
		float3 dir(1.0f, 0.3f, 0.5f);
		dir = normalize(dir);
		model = model * mat4::Rotate(dir.x, dir.y, dir.z, angle * TO_RADIANS);
		simpleShader->SetMat4x4("model", model);
		triangle.Draw();
	}

	simpleShader->Unbind();
}

void Game::KeyDown(XID key)
{
	switch (key)
	{
	case XK_w:
		yOffset -= 1;
		break;
	case XK_s:
		yOffset += 1;
		break;
	case XK_d:
		rotateCam.x += -1;
		break;
	case XK_a:
		rotateCam.x -= -1;
		break;
	case XK_z:
		rotateCam.y += 1;
		break;
	case XK_c:
		rotateCam.y -= 1;
		break;
	case XK_Left:
		moveCam.x = 1;


		break;
	case XK_Right:
		moveCam.x = -1;


		break;
	case XK_Down:
		moveCam.y = -1;


		break;
	case XK_Up:
		moveCam.y = 1;


		break;
	default:
		break;
	}
}

void Game::KeyUp(XID key)
{
	switch (key)
	{
	case XK_w:
		yOffset -= 1;

		break;
	case XK_s:
		yOffset += 1;
		break;
	case XK_d:
		rotateCam.x += 1;
		break;
	case XK_a:
		rotateCam.x -= 1;
		break;
	case XK_z:
		rotateCam.y += -1;
		break;
	case XK_c:
		rotateCam.y -= -1;
		break;
	case XK_Left:
		moveCam.x -= 1;


		break;
	case XK_Right:
		moveCam.x -= -1;


		break;
	case XK_Down:
		moveCam.y -= -1;


		break;
	case XK_Up:
		moveCam.y -= 1;


		break;
	default:
		break;
	}
	rotateCam.x = clamp(rotateCam.x, -1, 1);
	rotateCam.y = clamp(rotateCam.y, -1, 1);
	moveCam.x = clamp(moveCam.x, -1.0f, 1.0f);
	moveCam.y = clamp(moveCam.y, -1.0f, 1.0f);
}

void Game::MouseScroll(float x)
{
}

void Game::MouseDown(unsigned button)
{
}

void Game::MouseUp(unsigned button)
{
}

void Game::MouseMove(int x, int y)
{
}
