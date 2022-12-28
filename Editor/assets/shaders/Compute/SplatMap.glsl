#type compute
#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;

layout (binding = 0, rgba16f) uniform writeonly image2D uSplatMap;

uniform sampler2D uNormalMap;
uniform int uN;

void main()
{
	ivec2 x = ivec2(gl_GlobalInvocationID.xy);
	vec2 texCoord = gl_GlobalInvocationID.xy / float(uN);
	
	vec3 normal = normalize(texture(uNormalMap, texCoord).rgb);
	
	float slopeFactor = normal.z;

	vec4 blendValues = vec4(0.0f);

	if (slopeFactor > 0.5f)
	{
		blendValues.x = 1.0f;
	}
	else if (slopeFactor > 0.35f)
	{
		blendValues.y = 1.0f;
	}
	else
	{
		blendValues.z = 1.0f;
	}
	
	imageStore(uSplatMap, x, blendValues);
}
