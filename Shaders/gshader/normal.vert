#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
};

uniform mat4 model;

out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
    vs_out.normal = normalize((projection * view * model * vec4(aNormal, 0.0)).xyz);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
