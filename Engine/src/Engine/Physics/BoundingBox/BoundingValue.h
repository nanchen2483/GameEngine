#pragma once
#include <functional>
#include <glm/glm.hpp>

namespace Engine
{
	struct BoundingValue
	{
		glm::vec3 center;
		glm::vec3 extents;

		std::function<glm::vec3(glm::vec3)> GetSupportPoint;
	};
}
