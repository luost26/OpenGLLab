#version 410 core

layout (location = 0) out vec4 aAmbient;
layout (location = 1) out vec4 aDiffuse;
layout (location = 2) out vec4 aSpecular;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

struct Material {
    sampler2D texture_ambient0;
    sampler2D texture_diffuse0;
    sampler2D texture_specular0;

    float texture_ambient0_enabled;
    float texture_diffuse0_enabled;
    float texture_specular0_enabled;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
	float opacity;
};

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

	vec3 attenuation;
};

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
	vec3 direction;

	vec3 attenuation;
	vec2 cutoff;

	//float _padding1;
	//float _padding2;

	bool shadow_enabled;
	int shadow_map_index;
};

#define MAX_SHADOW_MAPS 12
uniform sampler2D shadowMaps[MAX_SHADOW_MAPS];
in vec4 FragPosLightSpaces[MAX_SHADOW_MAPS];

uniform Material material;

#define MAX_POINT_LIGHTS 1

uniform int numberOfPointLights;
layout (std140) uniform PointLightsBlock {
	PointLight pointLights[MAX_POINT_LIGHTS];
};

#define MAX_SPOT_LIGHTS 100

uniform int numberOfSpotLights;
layout (std140) uniform SpotLightsBlock {
	SpotLight spotLights[MAX_SPOT_LIGHTS];
};

in vec3 viewPos;

vec4 AmbientColor() {
    return texture(material.texture_ambient0, TexCoords) * material.texture_ambient0_enabled
    + vec4(material.ambient, 1.0) * (1 - material.texture_ambient0_enabled);
}

vec4 DiffuseColor() {
    return texture(material.texture_diffuse0, TexCoords) * material.texture_diffuse0_enabled
    + vec4(material.diffuse, 1.0) * (1 - material.texture_diffuse0_enabled);
}

vec4 SpecularColor() {
    return texture(material.texture_specular0, TexCoords) * material.texture_specular0_enabled
    + vec4(material.specular, 1.0) * (1 - material.texture_specular0_enabled);
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
    return 1.0f - clamp(shadowIntensity, 0.0f, 1.0f) * 1.02f;  // *1.02f : diminish light leaking
}

float CalculateShadowOfIndex(int idx, vec3 light_dir) {
	vec3 proj_coords = FragPosLightSpaces[idx].xyz / FragPosLightSpaces[idx].w;

	proj_coords = proj_coords * 0.5 + 0.5;
	float current_depth = proj_coords.z;

	if (proj_coords.z > 1.0)
        return 0.0;

	vec4 moments = ConvertOptimizedMoments(texture(shadowMaps[idx], proj_coords.xy));

    return 1.0f - CalculateMSMHamburger(moments, current_depth, 0.0000, 0.00003);
}

mat3 CalculatePointLightOfIndex(int idx) {
	PointLight light = pointLights[idx];

	float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist);

    /* Ambient */
    vec3 ambient = light.ambient * vec3(AmbientColor());

    /* Diffuse */
    vec3 light_dir = normalize(FragPos - light.position);
    float diff = max(dot(-light_dir, Normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(DiffuseColor());


    /* Specular */
    vec3 view_dir = normalize(FragPos - viewPos);
    // Blinn-Phong
    vec3 halfway_dir = normalize(-view_dir + -light_dir);

	float spec;
    if (material.shininess <= 0) spec = 0;
	else spec = pow(max(dot(Normal, halfway_dir), 0.0), material.shininess);
	
	// Specular intensity
    vec3 specular = light.specular * vec3(SpecularColor()) * spec;
	
	return mat3(ambient, diffuse*attenuation, specular*attenuation);
}

mat3 MergeAllPointLights() {
	mat3 result = mat3(0.0f);
	for (int i = 0; i < numberOfPointLights; ++ i) {
		result += CalculatePointLightOfIndex(i);
	}
	return result;
}

mat3 CalculateSpotLightOfIndex(int idx) {
	SpotLight light = spotLights[idx];

	float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist);

    /* Ambient */
    vec3 ambient = light.ambient * vec3(AmbientColor());

    /* Diffuse */
    vec3 light_dir = normalize(FragPos - light.position);
    float diff = max(dot(-light_dir, Normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(DiffuseColor());


    /* Specular */
    vec3 view_dir = normalize(FragPos - viewPos);
    // Blinn-Phong
    vec3 halfway_dir = normalize(-view_dir + -light_dir);

	float spec;
    if (material.shininess <= 0) spec = 0;
	else spec = pow(max(dot(Normal, halfway_dir), 0.0), material.shininess);

	// Specular intensity
    vec3 specular = light.specular * vec3(SpecularColor()) * spec;

	/* Spotlight cutoff */
	float theta = dot(light_dir, normalize(light.direction));
    float epsilon = light.cutoff.x - light.cutoff.y;
    float intensity = clamp((theta - light.cutoff.y) / epsilon, 0.0, 1.0);

	float shadow_intensity = 0.0f;
	if (light.shadow_enabled)
		shadow_intensity = CalculateShadowOfIndex(light.shadow_map_index, light_dir);

	return mat3(
		ambient*attenuation, 
		diffuse*attenuation*intensity*(1.0f-shadow_intensity),
		specular*attenuation*intensity*(1.0f-shadow_intensity)
	);
}

mat3 MergeAllSpotLights() {
	mat3 result = mat3(0.0f);
	for (int i = 0; i < numberOfSpotLights; ++ i) {
		result += CalculateSpotLightOfIndex(i);
	}
	return result;
}

in vec4 position;

void main()
{
    mat3 merged = MergeAllPointLights() + MergeAllSpotLights();
	aAmbient = vec4(merged[0], material.opacity);
	aDiffuse = vec4(merged[1], material.opacity);
	aSpecular = vec4(merged[2], material.opacity); // 1+0+0 = 1
}
