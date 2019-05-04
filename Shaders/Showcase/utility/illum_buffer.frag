#version 410 core

out vec4 color;
in vec2 TexCoord;

uniform sampler2D ambient;
uniform sampler2D diffuse;
uniform sampler2D specular;

void main()
{
    color = texture(ambient, TexCoord) + texture(diffuse, TexCoord) + texture(specular, TexCoord);
}