#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Skybox/skybox.h"

namespace Engine
{
	struct ENGINE_API SkyboxComponent : public IComponent
	{
		Ptr<Skybox> skybox = nullptr;
		// Editor-only
		std::vector<Ptr<Image>> images = std::vector<Ptr<Image>>(6);
	};
}
