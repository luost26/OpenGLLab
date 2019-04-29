#version 410 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

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

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
	vec3 attenuation;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
	vec3 direction;

	vec3 attenuation;
	vec2 cutoff;
};

uniform Material material;

#define MAX_POINT_LIGHTS 1

uniform int numberOfPointLights;
layout (std140) uniform PointLightsBlock {
	PointLight pointLights[MAX_POINT_LIGHTS];
};

#define MAX_SPOT_LIGHTS 100

uniform int numberOfSpotLights;
layout (std140) uniform SpotLightsBlock {
	SpotLight spotLights[MAX_SPOT_LIGHTS];
};

uniform vec3 viewPos;

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

vec3 CalculatePointLightOfIndex(int idx) {
	PointLight light = pointLights[idx];

	float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist);

    /* Ambient */
    vec3 ambient = light.ambient * vec3(AmbientColor());

    /* Diffuse */
    vec3 light_dir = normalize(FragPos - light.position);
    float diff = max(dot(-light_dir, Normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(DiffuseColor());


    /* Specular */
    vec3 view_dir = normalize(FragPos - viewPos);
    // Blinn-Phong
    vec3 halfway_dir = normalize(-view_dir + -light_dir);

	float spec;
    if (material.shininess <= 0) spec = 0;
	else spec = pow(max(dot(Normal, halfway_dir), 0.0), material.shininess);
	
	// Specular intensity
    vec3 specular = light.specular * vec3(SpecularColor()) * spec;
	
	return ambient + (diffuse + specular)*attenuation;
}

vec3 MergeAllPointLights() {
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < numberOfPointLights; ++ i) {
		result += CalculatePointLightOfIndex(i);
	}
	return result;
}

vec3 CalculateSpotLightOfIndex(int idx) {
	SpotLight light = spotLights[idx];

	float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist);

    /* Ambient */
    vec3 ambient = light.ambient * vec3(AmbientColor());

    /* Diffuse */
    vec3 light_dir = normalize(FragPos - light.position);
    float diff = max(dot(-light_dir, Normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(DiffuseColor());


    /* Specular */
    vec3 view_dir = normalize(FragPos - viewPos);
    // Blinn-Phong
    vec3 halfway_dir = normalize(-view_dir + -light_dir);

	float spec;
    if (material.shininess <= 0) spec = 0;
	else spec = pow(max(dot(Normal, halfway_dir), 0.0), material.shininess);

	// Specular intensity
    vec3 specular = light.specular * vec3(SpecularColor()) * spec;

	/* Spotlight cutoff */
	float theta = dot(light_dir, normalize(light.direction));
    float epsilon = light.cutoff.x - light.cutoff.y;
    float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);

    return ambient*attenuation + (diffuse + specular)*attenuation*intensity;
}

vec3 MergeAllSpotLights() {
	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < numberOfSpotLights; ++ i) {
		result += CalculateSpotLightOfIndex(i);
	}
	return result;
}

void main()
{
    FragColor = vec4(MergeAllPointLights() + MergeAllSpotLights(), 1.0);
}
