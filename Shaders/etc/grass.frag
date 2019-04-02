#version 410 core

in vec3 FragPos;
in vec2 TexCoord;

uniform sampler2D grass_texture;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(grass_texture, TexCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}
