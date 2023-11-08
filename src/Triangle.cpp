#include "Triangle.h"
#include <iostream>
#include "template.h"

#include "BasicVertexShader.h"
#include "BasicFragmentShader.h"

unsigned int shaderProgram[2];
unsigned int VAO1;
unsigned int VAO2;
unsigned int EBO;


void Triangle::Draw()
{
	glUseProgram(shaderProgram[0]);
	glBindVertexArray(VAO1);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(shaderProgram[1]);

	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Triangle::Init()
{
	//loading vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);


	glShaderSource(vertexShader, 1, &BasicVertexShader::vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//loading fragment shader

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &BasicFragmentShader::fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//loading fragment shader

	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = "#version 300 es\n"
		"out highp vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(0.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	glShaderSource(fragmentShader2, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader2);

	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//linking the shaders
	shaderProgram[0] = glCreateProgram();
	shaderProgram[1] = glCreateProgram();


	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader);
	glAttachShader(shaderProgram[1], fragmentShader2);
	glLinkProgram(shaderProgram[0]);
	glLinkProgram(shaderProgram[1]);

	glGetProgramiv(shaderProgram[0], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram[0], 512, NULL, infoLog);
		cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << infoLog << endl;
	}
	glGetProgramiv(shaderProgram[1], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram[1], 512, NULL, infoLog);
		cout << "ERROR::SHADER::LINK::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//freeing space
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//Vertex array object creation
	//first triangle
	glGenVertexArrays(1, &VAO1);

	glBindVertexArray(VAO1);

	//Vertex buffer object creation
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2Triangles), vertices2Triangles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	//Vertex array object creation
	//second triangle
	glGenVertexArrays(1, &VAO2);

	glBindVertexArray(VAO2);

	//Vertex buffer object creation
	unsigned int VBO1;
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1Triangles), vertices1Triangles, GL_STATIC_DRAW);
	////Element buffer object
	//glGenBuffers(1, &EBO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
