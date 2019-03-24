#version 410 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;
layout (location=2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform float alpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
//uniform mat4 transform;

void main() {
    gl_Position = proj * view * model * vec4(aPos.x*alpha, aPos.y*alpha, aPos.z*alpha, 1.0);
    ourColor = aColor;
    texCoord = aTexCoord;
}
