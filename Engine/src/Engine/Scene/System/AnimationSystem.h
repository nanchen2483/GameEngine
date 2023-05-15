#pragma once
#include "Engine/Renderer/Model/IAnimation.h"
#include "Engine/Renderer/Model/INode.h"

#include <entt/entt.hpp>

namespace Engine
{
	class AnimationSystem
	{
	public:
		static void OnUpdate(entt::registry& registry);
		static void UpdateAnimation(const Ptr<IAnimation>& animation);
	private:
		static void CalculateBoneTransform(const Ptr<IAnimation>& animation, const Ptr<INode>& node, glm::mat4 globalTransformation);
	};
}
