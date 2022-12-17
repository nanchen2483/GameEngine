#pragma once
#include "IComponent.h"

#include <glm/glm.hpp>
#include <vector>

namespace Engine
{
	struct LightComponent : public IComponent
	{
		float constant = 1.0f;
		float linear = 0.1f;
		float quadratic = 0.03f;

		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 ambient = glm::vec3(1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		const std::vector<const void*> GetData() const
		{
			return { &position, &constant, &linear, &quadratic, &ambient, &diffuse, &specular };
		}
	};
}
