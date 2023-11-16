#version 310 es
layout (location = 0) in vec3 aPos;
uniform vec3 offset;
out vec4 finalPos;
void main()
	{
		finalPos= vec4(offset + aPos,1.0);
		gl_Position = finalPos;
		
	}