#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

#define MAX_SHADOW_MAPS 12
uniform mat4 lightSpaces[MAX_SHADOW_MAPS];
out vec4 FragPosLightSpaces[MAX_SHADOW_MAPS];

uniform mat4 model;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
};

out vec4 position;

void main()
{
    TexCoords = aTexCoords;
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    FragPos = vec3(model * vec4(aPos, 1.0));
	for (int i = 0; i < MAX_SHADOW_MAPS; ++ i) {
		FragPosLightSpaces[i] = lightSpaces[i] * vec4(FragPos, 1.0);
	}

	position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos, 1.0);

}
