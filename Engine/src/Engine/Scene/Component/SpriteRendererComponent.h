#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <glm/glm.hpp>

namespace Engine
{
	struct ENGINE_API SpriteRendererComponent : public IComponent
	{
		glm::vec4 color = glm::vec4(1.0f);
		Ptr<Texture2D> texture = nullptr;
	};
}
