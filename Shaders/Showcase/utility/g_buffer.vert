#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
	vec3 viewPosition;
};

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = (view * worldPos).xyz;  // Important, don't forget to multipy `worldPos` by `view` matrix
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(view * model)));  // Important, same here, don't forget to multipy `model` by `view` matrix
    Normal = normalMatrix * aNormal;

    gl_Position = projection * view * worldPos;
}