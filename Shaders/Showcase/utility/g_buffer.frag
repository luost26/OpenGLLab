#version 410 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gSpecular;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

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
	float opacity;
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

const float NEAR = 0.1f;
const float FAR = 100.0f;
float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}


void main()
{    
    gPosition.xyz = FragPos;
	gPosition.a = LinearizeDepth(gl_FragCoord.z);
    gNormal = normalize(Normal);
    gAlbedo = DiffuseColor().rgb;
	gSpecular = SpecularColor().rgb;

	if (material.opacity < 0.9) discard;
}