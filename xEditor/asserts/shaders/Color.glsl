#type vertex
#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aColor;
layout(location = 3) in vec2 aTexCoord;
layout(location = 4) in float aTexIndex;
layout(location = 5) in vec3 aTangent;
layout(location = 6) in vec3 aBitangent;
layout(location = 7) in vec4 aBone1;
layout(location = 8) in vec4 aBone2;
layout(location = 9) in ivec4 aBoneIds;
layout(location = 10) in vec4 aWeights;
layout(location = 11) in int aEntityId;

uniform mat4 uViewProjection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec4 vColor;
out vec2 vTexCoord;
out float vTexIndex;
out flat int vEntityId;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
		int boneId = aBoneIds[i];
        if (boneId == -1)
		{
            continue;
		}

        if (boneId >= MAX_BONES)
        {
            totalPosition = vec4(aPosition, 1.0f);
            break;
        }

        vec4 localPosition = finalBonesMatrices[aBoneIds[i]] * vec4(aPosition, 1.0f);
        totalPosition += localPosition * aWeights[i];
	}

	if (totalPosition == vec4(0.0f))
	{
		totalPosition = vec4(aPosition, 1.0f);
	}
	
	vColor = aColor;
	vTexCoord = aTexCoord;
	vTexIndex = aTexIndex;
	vEntityId = aEntityId;
	gl_Position = uViewProjection * totalPosition;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityId;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexIndex;
in flat int vEntityId;

uniform sampler2D uTextures[32];

void main()
{
	FragColor = texture(uTextures[int(round(vTexIndex))], vTexCoord) * vColor;
	EntityId = vEntityId;
}
