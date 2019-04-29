#version 410 core

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

vec4 ConvertOptimizedMoments(vec4 opt_moments) {
	opt_moments.x -= 0.035955884801f;
	mat4 convert_mat = transpose(mat4(
			0.2227744146f, 0.1549679261f, 0.1451988946f, 0.163127443f,
			0.0771972861f, 0.1394629426f, 0.2120202157f, 0.2591432266f,
			0.7926986636f, 0.7963415838f, 0.7258694464f, 0.6539092497f,
			0.0319417555f,-0.1722823173f,-0.2758014811f,-0.3376131734f
		));
	vec4 converted = convert_mat * opt_moments;
	return converted;
}

float CalculateMSMHamburger(vec4 moments, float frag_depth, float depth_bias, float moment_bias) {
    // Bias input data to avoid artifacts
    vec4 b = mix(moments, vec4(0.5f, 0.5f, 0.5f, 0.5f), moment_bias);
    vec3 z;
    z[0] = frag_depth - depth_bias;

    // Compute a Cholesky factorization of the Hankel matrix B storing only non-
    // trivial entries or related products
    float L32D22 = fma(-b[0], b[1], b[2]);
    float D22 = fma(-b[0], b[0], b[1]);
    float squaredDepthVariance = fma(-b[1], b[1], b[3]);
    float D33D22 = dot(vec2(squaredDepthVariance, -L32D22), vec2(D22, L32D22));
    float InvD22 = 1.0f / D22;
    float L32 = L32D22 * InvD22;

    // Obtain a scaled inverse image of bz = (1,z[0],z[0]*z[0])^T
    vec3 c = vec3(1.0f, z[0], z[0] * z[0]);

    // Forward substitution to solve L*c1=bz
    c[1] -= b.x;
    c[2] -= b.y + L32 * c[1];

    // Scaling to solve D*c2=c1
    c[1] *= InvD22;
    c[2] *= D22 / D33D22;

    // Backward substitution to solve L^T*c3=c2
    c[1] -= L32 * c[2];
    c[0] -= dot(c.yz, b.xy);

    // Solve the quadratic equation c[0]+c[1]*z+c[2]*z^2 to obtain solutions
    // z[1] and z[2]
    float p = c[1] / c[2];
    float q = c[0] / c[2];
    float D = (p * p * 0.25f) - q;
    float r = sqrt(D);
    z[1] =- p * 0.5f - r;
    z[2] =- p * 0.5f + r;

    // Compute the shadow intensity by summing the appropriate weights
    vec4 switchVal = (z[2] < z[0]) ? vec4(z[1], z[0], 1.0f, 1.0f) :
                      ((z[1] < z[0]) ? vec4(z[0], z[1], 0.0f, 1.0f) :
                      vec4(0.0f,0.0f,0.0f,0.0f));
    float quotient = (switchVal[0] * z[2] - b[0] * (switchVal[0] + z[2]) + b[1])/((z[2] - switchVal[1]) * (z[0] - z[1]));
    float shadowIntensity = switchVal[2] + switchVal[3] * quotient;
    return 1.0f - clamp(shadowIntensity, 0.0f, 1.0f);
}