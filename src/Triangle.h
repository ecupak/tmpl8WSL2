#pragma once
struct Triangle
{
	void Draw();
	void Init();

private:
	float vertices[12] = {
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left 
	};
	float vertices1Triangles[9] = {
		-0.5f, 0.5f, 0.0f, // top right
		0.0f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left

	};
	float vertices2Triangles[9] = {

		0.5f, 0.5f, 0.0f, // top left
		0.2f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
	};
	unsigned int indices[6] = {
		// note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle
	};
};
