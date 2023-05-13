#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Animation.h"

namespace Engine
{
	struct ENGINE_API AnimationComponent : public IComponent
	{
		bool isEnabled = false;
		uint32_t selectedAnimationIndex = 0;

		std::vector<Ptr<Animation>> animations;

		operator Ptr<Animation>()
		{
			if (!animations.empty())
			{
				return animations[selectedAnimationIndex];
			}

			return nullptr;
		}
	};
}
