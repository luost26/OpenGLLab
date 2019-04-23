#version 410 core

out vec4 color;
in vec2 TexCoord;

uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, TexCoord).r;
    color = vec4(vec3(depthValue), 1.0);
}