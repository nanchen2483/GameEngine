#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	namespace DirectionalLightConstant
	{
		const glm::vec3 Direction = glm::normalize(glm::vec3(0.2f, 1.0f, 0.3f));
		const glm::vec3 Ambient = glm::vec3(0.05f);
		const glm::vec3 Diffuse = glm::vec3(0.4f);
		const glm::vec3 Specular = glm::vec3(0.5f);
	}
}
