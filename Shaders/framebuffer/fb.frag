#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D screenTexture;

const float x_offset = (1.0/800.0)*3;
const float y_offset = (1.0/600.0)*3;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-x_offset, -y_offset),
        vec2(0, -y_offset),
        vec2(+x_offset, -y_offset),
        vec2(-x_offset, 0),
        vec2(0, 0),
        vec2(+x_offset, 0),
        vec2(-x_offset, +y_offset),
        vec2(0, +y_offset),
        vec2(+x_offset, +y_offset)
    );
    
    float kernel[9] = float[](
                              -1, -1, -1,
                              -1,  9, -1,
                              -1, -1, -1
                              );
    
    if (gl_FragCoord.x < 800) {
        vec3 col = vec3(0,0,0);
        for (int i = 0; i < 9; ++ i) {
            col += kernel[i] * texture(screenTexture, TexCoord+offsets[i]).rgb;
        }
        FragColor = vec4(col, 1.0);
    } else {
        FragColor = vec4(texture(screenTexture, TexCoord).rgb, 1.0);
    }
}
