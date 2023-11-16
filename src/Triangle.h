#pragma once
struct Triangle
{
	void Draw();
	void Init();

private:
	float vertices[9] = {
		0.0f, 0.5f, 0.0f, // top 
		0.5f, -0.5f, 0.0f, //down right
		-0.5f, -0.5f, 0.0f //down left
	};
	unsigned int indices[6] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};
};
