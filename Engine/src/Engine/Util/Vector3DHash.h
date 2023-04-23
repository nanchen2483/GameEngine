#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	class Vector3DHash
	{
	public:
		size_t operator()(const glm::dvec3& v) const
		{
			return std::hash<double>()(v.x) ^ std::hash<double>()(v.y) ^ std::hash<double>()(v.z);
		}
	};
}
