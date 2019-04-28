#version 410 core

in vec3 FragPos;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D color_texture;
uniform sampler2D shadowMap;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight light;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float currentDepth = projCoords.z;
    float bias = max(0.05 * (1.0 - dot(normal, -lightDir)), 0.005);

    vec2 textel_size = 1.0 / textureSize(shadowMap, 0);
    float shadow = 0.0;
    int radius = 2;
    for (int x = -radius; x <= radius; ++ x) {
        for (int y = -radius; y <= radius; ++ y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x,y)*textel_size).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0: 0.0;
        }
    }

    shadow /= (2*radius+1)*(2*radius+1);

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 CalculatePointLight() {

    vec3 normal = vec3(0.0, 1.0, 0.0);
    vec3 color = texture(color_texture, TexCoord).rgb;
    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    /* Ambient */
    vec3 ambient = light.ambient * color;

    /* Diffuse */
    vec3 light_dir = normalize(FragPos - light.position);
    float diff = max(dot(-light_dir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * color;

    /* Specular */
    vec3 view_dir = normalize(FragPos - viewPos);
    // Blinn-Phong
    vec3 halfway_dir = normalize(-view_dir + -light_dir);
    float spec = pow(max(dot(normal, halfway_dir), 0.0), 32.0);
    // Phong
    //vec3 reflect_dir = reflect(light_dir, normal);
    //float spec = pow(max(dot(-view_dir, reflect_dir), 0.0), 1.0);
    vec3 specular = light.specular * spec;

    return (ambient + (1.0-ShadowCalculation(FragPosLightSpace, normal, light_dir)) * (diffuse + specular) ) * attenuation;
}

void main() {
    FragColor = vec4(CalculatePointLight(), 1.0);
}
