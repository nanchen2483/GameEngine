#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	struct TerrainVertex
	{
		TerrainVertex() = default;

		glm::vec3 position = {};
		glm::vec2 texCoord = {};

		// Editor-only
		int32_t entityId = -1;
	};
}
