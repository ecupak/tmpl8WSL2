#pragma once
#include "template.h"

class Texture
{
public:
	Texture(Surface* texture);
	~Texture();
	void Init(const char* filePath, bool alpha = false);
	void Bind();
	void Unbind();

private:
	Surface* texture = nullptr;
	uint ID = 0;
};
