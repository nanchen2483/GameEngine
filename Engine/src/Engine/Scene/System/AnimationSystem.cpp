#include "enginepch.h"
#include "AnimationSystem.h"
#include "Engine/Core/System/System.h"
#include "Engine/Scene/Component/AnimationComponent.h"

namespace Engine
{
	void AnimationSystem::OnUpdate(entt::registry& registry)
	{
		registry.view<AnimationComponent>()
			.each([&](AnimationComponent& component)
				{
					if (component.isEnabled)
					{
						UpdateAnimation(component);
					}
				});
	}

	void AnimationSystem::UpdateAnimation(const Ptr<Animation>& animation)
	{
		Ptr<float> animationTime = animation->GetTime();
		*animationTime += animation->GetTicketPerSecond() * System::GetDeltaTime();
		*animationTime = fmod(*animationTime, animation->GetDuration());

		CalculateBoneTransform(animation, animation->GetRootNode(), glm::mat4(1.0f));
	}

	void AnimationSystem::CalculateBoneTransform(const Ptr<Animation>& animation, const Ptr<Node>& node, glm::mat4 globalTransformation)
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

