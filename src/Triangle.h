#pragma once
#include "Texture.h"


class Triangle
{
public:
	void Draw();
	void Init();

private:
	Texture wall = Texture(new Surface("assets/wall.jpg"));
	Texture face = Texture(new Surface("assets/awesomeface.png"));

	unsigned int indices[6] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};
	float vertices[32] = {
		// positions          // colors           // texture coords
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left 
	};
};
