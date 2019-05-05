#version 410 core

out vec4 color;
in vec2 TexCoord;

uniform sampler2D ambient;
uniform sampler2D diffuse;
uniform sampler2D specular;

uniform int AOEnabled = 0;
uniform sampler2D AOTexture;

void main()
{
	float ambient_diminish = AOEnabled == 1 ? texture(AOTexture, TexCoord).r : 1.0;
    color = texture(ambient, TexCoord) * ambient_diminish + texture(diffuse, TexCoord) + texture(specular, TexCoord);
}