#pragma once

#include <glm/glm.hpp>

#define MAX_BONE_WEIGHTS 4
#define UNINITIALIZED_BONE_ID -1

namespace Engine
{
	struct Vertex
	{
		glm::vec3 position = {};
		glm::vec3 normal = {};
		glm::vec4 color = {};
		glm::vec2 texCoord = {};
		float textureIndex = 0;
		glm::vec3 tangent = {};
		glm::vec3 bitangent = {};
		glm::vec4 bone1 = {};
		glm::vec4 bone2 = {};
		int boneIds[MAX_BONE_WEIGHTS] = {};
		float weights[MAX_BONE_WEIGHTS] = {};

		// Editor-only
		int entityId = -1;

		Vertex()
		{
			for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
			{
				boneIds[i] = UNINITIALIZED_BONE_ID;
				weights[i] = 0.0f;
			}
		}
	};
}
