#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Texture/ITexture.h"

#include <glm/glm.hpp>

namespace Engine
{
	struct ENGINE_API SpriteRendererComponent : public IComponent
	{
		glm::vec4 color{ 1.0f };
		Ptr<ITexture2D> texture{};
	};
}
