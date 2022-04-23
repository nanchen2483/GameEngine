#pragma once

#include <glm/glm.hpp>
#include "Buffer/BufferLayout.h"

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
		glm::vec3 material = {};
		glm::vec3 tangent = {};
		glm::vec3 bitangent = {};
		int boneIds[MAX_BONE_WEIGHTS] = {};
		float weights[MAX_BONE_WEIGHTS] = {};
		uint32_t isWorldPos = false;
		uint32_t hasAnimations = false;

		// Editor-only
		int entityId = -1;

		Vertex()
		{
			for (uint32_t i = 0; i < MAX_BONE_WEIGHTS; i++)
			{
				boneIds[i] = UNINITIALIZED_BONE_ID;
				weights[i] = 0.0f;
			}
		}

		bool SetBone(const uint32_t boneId, const float weight)
		{
			for (uint32_t i = 0; i < MAX_BONE_WEIGHTS; i++)
			{
				if (boneIds[i] == UNINITIALIZED_BONE_ID)
				{
					boneIds[i] = boneId;
					weights[i] = weight;
					return true;
				}
			}

			return false;
		}

		static const BufferLayout GetBufferLayout()
		{
			return BufferLayout(
				BufferLayoutType::Default,
				{
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float4 },
					{ ShaderDataType::Float2 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Float3 },
					{ ShaderDataType::Int4 },
					{ ShaderDataType::Float4 },
					{ ShaderDataType::Int },
					{ ShaderDataType::Int },
					{ ShaderDataType::Int }
				}
			);
		}
	};
}
