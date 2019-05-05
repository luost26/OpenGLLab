#version 410 core

out vec4 color;
in vec2 TexCoord;

uniform sampler2D screenTexture;

void main()
{
	float red=(texture(screenTexture, TexCoord)).r;
    color = vec4(red, red, red, 1.0);
}