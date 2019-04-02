#version 410 core

in vec3 FragPos;
in vec2 TexCoord;

struct PointLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;

uniform PointLight point_light;

uniform sampler2D color_texture;
uniform sampler2D rough_texture;
uniform sampler2D norm_texture;
uniform sampler2D occ_texture;

out vec4 FragColor;

uniform mat4 model;

vec3 apply_point_light(PointLight light) {
    vec3 ambient = light.ambient * texture(color_texture, TexCoord).rgb * texture(occ_texture, TexCoord).rgb;
    
    vec3 texNorm = normalize((texture(norm_texture, TexCoord).xyz-vec3(0.5) ));
    vec3 norm = vec3(texNorm.x, texNorm.z, -texNorm.y);
    vec3 lightDir = normalize(FragPos - light.position);
    vec3 diffuse = max(dot(-norm, lightDir), 0.0) * light.diffuse * texture(color_texture, TexCoord).rgb;
    
    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 reflectDir = reflect(lightDir, norm);
    
    float spec = pow(max(-dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * light.specular * texture(rough_texture, TexCoord).rgb * texture(color_texture, TexCoord).rgb;
    
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    
    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    FragColor = vec4(apply_point_light(point_light), 1.0);
//    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
