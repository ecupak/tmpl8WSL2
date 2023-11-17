#version 310 es
out highp vec4 FragColor;

in highp vec3 ourColor;
in highp vec2 TexCoord;

uniform sampler2D wallTexture;
uniform sampler2D faceTexture;
uniform highp float mixing;
void main()
	{ 
		highp vec2 faceCoord = vec2(1.0 - TexCoord.x,TexCoord.y);
		FragColor = mix(texture(wallTexture, TexCoord), texture(faceTexture, faceCoord), mixing);
	}