#include "enginepch.h"
#include "AnimationSystem.h"
#include "Engine/Core/System/System.h"

namespace Engine
{
	void AnimationSystem::UpdateAnimation(Ptr<Animation> animation)
	{
		Ptr<float> animationTime = animation->GetTime();
		*animationTime += animation->GetTicketPerSecond() * System::GetDeltaTime();
		*animationTime = fmod(*animationTime, animation->GetDuration());

		CalculateBoneTransform(animation, animation->GetRootNode(), glm::mat4(1.0f));
	}

	void AnimationSystem::CalculateBoneTransform(Ptr<Animation> animation, const Ptr<Node> node, glm::mat4 globalTransformation)
	{
		globalTransformation *= node->GetTransform(*animation->GetTime());
		if (node->AnyBones())
		{
			animation->UpdateBoneTransforms(node->GetBoneId(), globalTransformation * node->GetBoneOffset());
		}

		for (uint32_t i = 0; i < node->GetNumOfChildNodes(); i++)
		{
			CalculateBoneTransform(animation, node->GetChildNode(i), globalTransformation);
		}
	}
}

