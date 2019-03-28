#version 410 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    
    vec3 specular;
};

struct FlashLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    
    float cosphi;
    float outercosphi;
    
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
uniform Light light;
uniform DirectionalLight dirLight;
uniform FlashLight fLight;

vec3 calcPointLight(Light light) {
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
    
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(FragPos - light.position);
    vec3 diffuse = max(dot(-norm, lightDir), 0.0) * light.diffuse * texture(material.diffuse, TexCoord).rgb;
    
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 reflectDir = reflect(lightDir, norm);
    
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * texture(material.specular, TexCoord).rgb;
    
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    
    return (ambient + diffuse + specular) * attenuation;
}

vec3 calcDirectionalLight(DirectionalLight dirLight) {
    vec3 norm = normalize(Normal);
    vec3 dirLightDir = normalize(dirLight.direction);
    vec3 dirDiffuse = max(dot(-norm, dirLightDir), 0.0) * dirLight.diffuse * texture(material.diffuse, TexCoord).rgb;
    
    return dirDiffuse;
}

vec3 calcFlashLight(FlashLight light) {
    vec3 lightDir = normalize(FragPos - light.position);

    float theta = dot(lightDir, normalize(light.direction));
    float epsilon = light.cosphi - light.outercosphi;
    float intensity = clamp((theta - light.outercosphi) / epsilon, 0.0, 1.0);
    
    Light convLight;
    convLight.ambient = vec3(0.0);
    convLight.diffuse = light.diffuse;
    convLight.specular = light.specular;
    convLight.position = light.position;
    convLight.constant = light.constant;
    convLight.linear = light.linear;
    convLight.quadratic = light.quadratic;
    
    return calcPointLight(convLight) * intensity;
}

void main() {
    
    /* Point Light */
    vec3 pl = calcPointLight(light);
    
    /* Directional Light */
    vec3 dl = calcDirectionalLight(dirLight);
    
    /* Flash Light */
    vec3 fl = calcFlashLight(fLight);
    
    /* Emission */
    vec3 emission = texture(material.emission, TexCoord).rgb;
    
    vec3 result = pl + dl + fl;
    FragColor = vec4(result, 1.0);
}
