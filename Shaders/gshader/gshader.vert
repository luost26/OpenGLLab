#version 410 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out VS_OUT {
    vec3 color;
} vs_out;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    gl_PointSize = 100.0;
    vs_out.color = aColor;
}
