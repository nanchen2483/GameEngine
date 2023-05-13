#pragma once
#include "Engine/Core/Base.h"

#include <glm/glm.hpp>

namespace Engine
{
	class ENGINE_API Math
	{
	public:
		static bool DecomposeTransform(const glm::mat4 transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

		template <typename T>
		static int32_t Sign(T value)
		{
			return (T(0) < value) - (value < T(0));
		}
	};
}
