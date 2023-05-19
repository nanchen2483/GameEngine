#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Animation/IAnimation.h"

namespace Engine
{
	struct ENGINE_API AnimationComponent : public IComponent
	{
		bool isEnabled = false;
		uint32_t selectedAnimationIndex = 0;

		std::vector<Ptr<IAnimation>> animations;

		operator Ptr<IAnimation>()
		{
			if (!animations.empty())
			{
				return animations[selectedAnimationIndex];
			}

			return nullptr;
		}
	};
}
