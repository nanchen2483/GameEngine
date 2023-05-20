#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	struct CollisionInfo
	{
		bool isCollided{};
		double penetrationDepth{};
		glm::dvec3 collisionNormal{};

		// Test-only
		double separation{};
		uint32_t iterations{};
	};
}
