#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
};

uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse0, TexCoords) * 1 + texture(material.texture_specular0, TexCoords)*0.0;
}
