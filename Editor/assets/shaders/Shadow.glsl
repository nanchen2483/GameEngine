#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 7) in ivec4 aBoneIds;
layout (location = 8) in vec4 aWeights;
layout (location = 9) in int aIsWorldPos;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 uModel;
uniform bool uHasAnimation;
uniform mat4 uBoneTransforms[MAX_BONES];

vec4 CalcWorldPosition();
vec4 CalcBonePosition();

void main()
{
	gl_Position = CalcWorldPosition();
}

vec4 CalcWorldPosition()
{
	vec4 position = vec4(0.0f);
	if (uHasAnimation == true)
	{
		position = CalcBonePosition();
	}
	else
	{
		position = vec4(aPosition, 1.0f);
	}

	if (aIsWorldPos == 0)
	{
		position = uModel * position;
	}

	return position;
}

vec4 CalcBonePosition()
{
	vec4 bonePosition = vec4(0.0f);
	for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
	{
		int boneId = aBoneIds[i];
		if (boneId == -1)
		{
			continue;
		}

		if (boneId >= MAX_BONES)
		{
			return vec4(aPosition, 1.0f);
		}

		bonePosition += uBoneTransforms[boneId] * vec4(aPosition, 1.0f) * aWeights[i];
	}

	return bonePosition;
}

#type geometry
#version 460 core

layout (triangles, invocations = 5) in;
layout (triangle_strip, max_vertices = 3) out;

layout (std140, binding = 3) uniform LightSpaceBlock
{
	mat4 lightSpaceMatrices[16];
} uLightSpace;

void main()
{
	for (int i = 0; i < 3; ++i)
	{
		gl_Position = uLightSpace.lightSpaceMatrices[gl_InvocationID] * gl_in[i].gl_Position;
		gl_Layer = gl_InvocationID;
		EmitVertex();
	}

	EndPrimitive();
}

#type fragment
#version 450 core

void main()
{
}
