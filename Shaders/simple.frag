#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 texCoord;

uniform float alpha;
uniform sampler2D texture0;

void main() {
//    FragColor = texture(texture0, texCoord) * vec4(alpha, ourColor);
    FragColor = texture(texture0, texCoord);

}
