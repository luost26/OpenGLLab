#version 410 core

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D color_texture;
uniform sampler2D specular_texture;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;

    float shininess;
};

uniform PointLight light;
uniform Material material;
uniform vec3 viewPos;

vec3 CalculatePointLight() {

    vec3 normal = normalize(Normal);
    vec3 color = texture(material.texture_diffuse0, TexCoord).rgb;
    vec3 specular_color = texture(material.texture_specular0, TexCoord).rgb;

    /* Ambient */
    vec3 ambient = light.ambient * color;

    /* Diffuse */
    vec3 light_dir = normalize(FragPos - light.position);
    float diff = max(dot(-light_dir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * color;

    /* Specular */
    vec3 view_dir = normalize(FragPos - viewPos);
    vec3 reflect_dir = reflect(light_dir, normal);
    float spec = pow(max(dot(-view_dir, reflect_dir), 0.0), 1.0);
    vec3 specular = light.specular * spec * specular_color;

    return ambient + diffuse + specular;
}

void main() {
    FragColor = vec4(CalculatePointLight(), 1.0);
}
