#version 410 core

layout (location = 0) vec3 aPos;
layout (location = 1) vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT {
    vec3 normal;
} vs_out;

void main() {
    vs_out.normal = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
