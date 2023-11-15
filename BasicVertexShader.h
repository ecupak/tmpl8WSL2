#pragma once
class BasicVertexShader
{
public:
	BasicVertexShader();
	inline static const char* vertexShaderSource = "#version 310 es\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
};
