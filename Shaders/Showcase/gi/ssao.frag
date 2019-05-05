#version 330 core
out float FragColor;
in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

int kernelSize = 64;
float radius = 1.0;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1366.0f/4.0f, 768.0f/4.0f); 


layout (std140) uniform Camera {
    mat4 projection;
    mat4 view;
	vec3 viewPosition;
};

void main()
{


    vec3 fragPos = (view * vec4(texture(gPosition, TexCoord).xyz, 1.0f)).xyz;
    vec3 normal = view * texture(gNormal, TexCoord).rgb;
    vec3 randomVec = texture(texNoise, TexCoord * noiseScale).xyz;

    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {

        vec3 sample = TBN * samples[i]; // From tangent to view-space
        sample = fragPos + sample * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = -texture(gPosition, offset.xy).w; // Get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth ));
        occlusion += (sampleDepth >= sample.z ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}