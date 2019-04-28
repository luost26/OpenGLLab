#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_ambient0;
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;

    float texture_ambient0_enabled;
    float texture_diffuse0_enabled;
    float texture_specular0_enabled;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

vec4 AmbientColor() {
    return texture(material.texture_ambient0, TexCoords) * material.texture_ambient0_enabled
            + vec4(material.ambient, 1.0) * (1 - material.texture_ambient0_enabled);
}

vec4 DiffuseColor() {
    return texture(material.texture_diffuse0, TexCoords) * material.texture_diffuse0_enabled
            + vec4(material.diffuse, 1.0) * (1 - material.texture_diffuse0_enabled);
}

vec4 SpecularColor() {
    return texture(material.texture_specular0, TexCoords) * material.texture_specular0_enabled
            + vec4(material.specular, 1.0) * (1 - material.texture_specular0_enabled);
}

void main()
{
    FragColor = DiffuseColor();
}
