#version 330 core

vec4 GetOptimizedMoments(float depth) {
	float squared = depth * depth;
	vec4 moments  = vec4(depth, squared, depth*squared, squared*squared);
	
	mat4 optimize_mat = transpose(mat4(
			-2.07224649f,    13.7948857237f,  0.105877704f,   9.7924062118f,
            32.23703778f,  -59.4683975703f, -1.9077466311f, -33.7652110555f,
            -68.571074599f,  82.0359750338f,  9.3496555107f,  47.9456096605f,
            39.3703274134f,-35.364903257f,  -6.6543490743f, -23.9728048165f	
		));

	vec4 optimized = optimize_mat * moments;
	optimized.x += 0.035955884801f;

	return optimized;
}

out vec4 FragColor;

void main()
{
	float depth = gl_FragCoord.z;
	FragColor = GetOptimizedMoments(depth);

}