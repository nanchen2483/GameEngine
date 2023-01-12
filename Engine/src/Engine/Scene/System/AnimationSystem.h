#pragma once
#include "Engine/Renderer/Model/Animation.h"
#include "Engine/Renderer/Model/Node.h"

namespace Engine
{
	class AnimationSystem
	{
	public:
		static void UpdateAnimation(Ptr<Animation> animation);
	private:
		static void CalculateBoneTransform(Ptr<Animation> animation, const Ptr<Node> node, glm::mat4 globalTransformation);
	};
}
