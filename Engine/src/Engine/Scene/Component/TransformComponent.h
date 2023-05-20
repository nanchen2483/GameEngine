#pragma once
#include "IComponent.h"
#include "Engine/Math/Transform.h"

namespace Engine
{
	struct ENGINE_API TransformComponent : public IComponent
	{
		Transform transform{};

		glm::vec3 GetTranslation() const { return transform.translation; }
		glm::vec3 GetRotation() const { return transform.rotation; }
		glm::vec3 GetScale() const { return transform.scale; }

		operator glm::mat4() const { return transform; }
		operator Transform& () { return transform; }
	};
}
