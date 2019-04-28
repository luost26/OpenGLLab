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

    float constant;
    float linear;
    float quadratic;
};

uniform Material material;

uniform PointLight light;
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

vec3 CalculatePointLight() {

    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

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
    float spec = pow(max(dot(Normal, halfway_dir), 0.0), material.shininess);

    vec3 specular = light.specular * vec3(SpecularColor()) * spec;

    return ambient + (diffuse + specular)*attenuation;
}


void main()
{
    FragColor = vec4(CalculatePointLight(), 1.0);
}
