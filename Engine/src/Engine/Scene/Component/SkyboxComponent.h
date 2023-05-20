#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Skybox/Skybox.h"

namespace Engine
{
	struct ENGINE_API SkyboxComponent : public IComponent
	{
		Ptr<Skybox> skybox{};
		// Editor-only
		std::vector<Ptr<Image>> images = std::vector<Ptr<Image>>(6);
	};
}
