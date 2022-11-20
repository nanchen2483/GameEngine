#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4 transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

		template <typename T>
		int Sign(T value)
		{
			return (T(0) < value) - (value < T(0));
		}
	}
}
