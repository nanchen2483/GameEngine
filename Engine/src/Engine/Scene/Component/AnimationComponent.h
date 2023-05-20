#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Animation/IAnimation.h"

namespace Engine
{
	struct ENGINE_API AnimationComponent : public IComponent
	{
		bool isEnabled{};
		uint32_t selectedAnimationIndex{};
		std::vector<Ptr<IAnimation>> animations{};

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
