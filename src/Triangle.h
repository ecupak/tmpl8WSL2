#pragma once
struct Triangle
{
	void Draw();
	void Init();
	const float vertices[9] = {
		-0.5f, -0.2f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};
};
