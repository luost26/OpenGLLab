#version 410 core

out vec4 color;
in vec2 TexCoord;

uniform sampler2D screenTexture;

vec4 ConvertOptimizedMoments(vec4 opt_moments) {
	const float SQRT3 = 1.732050807568877f;
	opt_moments -= vec4(0.5f, 0.0f, 0.5f, 0.0f);
	mat4 convert_mat = mat4(
			-1.0f/3.0f,	0.0f,	SQRT3,	0.0f,
			0.0f,	0.125f,		0.0f,	1.0f,
			-0.75f,	0.0f,	0.75f*SQRT3, 0.0f,
			0.0f,	-0.125f,	0.0f,	1.0f
		);
	vec4 converted = convert_mat * opt_moments;
	return converted;
}

void main()
{
	float red=(texture(screenTexture, TexCoord)).r;
    //color = vec4( (normalize(texture(screenTexture, TexCoord)).rgb + vec3(1.0, 1.0, 1.0)) / 2.0, 1.0);
	color = vec4(red,red,red,1.0);
	//color = ConvertOptimizedMoments(texture(screenTexture, TexCoord));
}