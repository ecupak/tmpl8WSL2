#pragma once
class BasicFragmentShader
{
public:
	BasicFragmentShader();
	inline static const char* fragmentShaderSource = "#version 310 es\n"
		"out highp vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
};
