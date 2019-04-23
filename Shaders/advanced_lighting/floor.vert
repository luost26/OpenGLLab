#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out vec2 TexCoord;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
}