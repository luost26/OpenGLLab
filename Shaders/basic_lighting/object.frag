#version 410 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
};

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;


void main() {
    vec3 ambient = light.ambient * material.ambient;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(FragPos - light.position);
    vec3 diffuse = max(dot(-norm, lightDir), 0.0) * light.diffuse * material.diffuse;
    
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 reflectDir = reflect(lightDir, norm);
    
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * material.specular;
    
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
