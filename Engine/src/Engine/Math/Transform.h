#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	struct Transform
	{
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		operator glm::mat4() const
		{
			return glm::translate(glm::mat4(1.0f), translation) *
				   glm::toMat4(glm::quat(rotation)) *
				   glm::scale(glm::mat4(1.0f), scale);
		}
	};
}
