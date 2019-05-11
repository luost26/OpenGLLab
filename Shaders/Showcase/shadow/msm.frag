#version 330 core
layout(location = 0) out vec4 outOptimizedMoments;
layout(location = 1) out vec4 outAlpha;

vec4 GetOptimizedMoments(float depth) {
    const float SQRT3 = 1.732050807568877f;
	float squared = depth * depth;
	vec4 moments  = vec4(depth, squared, depth*squared, squared*squared);
	
	mat4 optimize_mat = mat4(
			1.5f,	0.0f,	-2.0f,	0.0f,
			0.0f,	4.0f,	0.0f,	-4.0f,
			SQRT3/2.0f, 0.0f, -2*SQRT3/9.0f, 0.0f,
			0.0f,	0.5f,	0.0f,	0.5f
		);

	vec4 optimized = optimize_mat * moments;
	optimized += vec4(0.5f, 0.0f, 0.5f, 0.0f);

	return optimized;
}

out vec4 FragColor;

void main()
{
	float depth = gl_FragCoord.z;
	FragColor = GetOptimizedMoments(depth);
}