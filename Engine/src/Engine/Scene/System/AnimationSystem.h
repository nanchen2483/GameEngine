#pragma once
#include "Engine/Renderer/Model/Animation.h"
#include "Engine/Renderer/Model/Node.h"

#include <entt/entt.hpp>

namespace Engine
{
	class AnimationSystem
	{
	public:
		static void OnUpdate(entt::registry& registry);
		static void UpdateAnimation(const Ptr<Animation>& animation);
	private:
		static void CalculateBoneTransform(const Ptr<Animation>& animation, const Ptr<Node>& node, glm::mat4 globalTransformation);
	};
}
