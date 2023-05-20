#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	struct Transform
	{
		glm::vec3 translation{ 0.0f };
		glm::vec3 rotation{ 0.0f };
		glm::vec3 scale{ 1.0f };
		glm::vec3 velocity{ 0.0f };

		operator glm::mat4() const
		{
			return glm::translate(glm::mat4(1.0f), translation) *
				   glm::toMat4(glm::quat(rotation)) *
				   glm::scale(glm::mat4(1.0f), scale);
		}
	};
}
