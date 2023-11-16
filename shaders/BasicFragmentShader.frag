#version 310 es
out highp vec4 FragColor;
in highp vec4 finalPos;

void main()
	{
		FragColor = vec4(finalPos.x, finalPos.y, finalPos.z, 1.0f);
	}