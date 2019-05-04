#version 410 core

out vec4 color;
in vec2 TexCoord;

uniform int samples;

uniform sampler2DMS ambient;
uniform sampler2DMS diffuse;
uniform sampler2DMS specular;

void main()
{
	vec4 c = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 dimension = textureSize(ambient);
	ivec2 coord = ivec2(dimension.x * TexCoord.x, dimension.y * TexCoord.y);
	for (int i = 0; i < samples; ++ i) {	
		c += texelFetch(ambient, coord, i) + texelFetch(diffuse, coord, i) + texelFetch(specular, coord, i);
	}
	color = c / samples;
}