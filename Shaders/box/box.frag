#version 410 core

struct Material {
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;

uniform Material material;
uniform PointLight pointLight;

vec3 calcPointLight(PointLight light) {
    vec3 ambient = light.ambient * texture(material.texture_diffuse0, TexCoord).rgb;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(FragPos - light.position);
    vec3 diffuse = max(dot(-norm, lightDir), 0.0) * light.diffuse * texture(material.texture_diffuse0, TexCoord).rgb;
    
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 reflectDir = reflect(lightDir, norm);
    
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * texture(material.texture_specular0, TexCoord).rgb;
    
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    
    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    
    /* Point Light */
    vec3 pl = calcPointLight(pointLight);
    
    vec3 result = pl;
    FragColor = vec4(result, 1.0);
}
