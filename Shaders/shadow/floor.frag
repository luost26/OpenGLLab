#version 410 core

in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D color_texture;

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

    return (ambient + diffuse + specular) * attenuation;
}

void main() {
    FragColor = vec4(CalculatePointLight(), 1.0);
}
