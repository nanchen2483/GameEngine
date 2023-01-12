#include "enginepch.h"
#include "AssimpAnimation.h"

#include "Engine/Core/System/System.h"

namespace Engine
{
	AssimpAnimation::AssimpAnimation(const aiAnimation* animation, const Ptr<Node> rootNode)
		:	m_name(animation->mName.data),
			m_animationTime(CreatePtr<float>(0.0f)),
			m_duration((float)animation->mDuration),
			m_ticksPerSecond((float)animation->mTicksPerSecond),
			m_rootNode(rootNode)
	{
		m_boneTransforms.reserve(100);
		for (uint32_t i = 0; i < 100; i++)
		{
			m_boneTransforms.push_back(glm::mat4(1.0f));
		}
	}

	void AssimpAnimation::UpdateBoneTransforms()
	{
		CalculateAnimationTime();
		CalculateBoneTransform(m_rootNode, glm::mat4(1.0f));
	}

	void AssimpAnimation::CalculateAnimationTime()
	{
		*m_animationTime += m_ticksPerSecond * System::GetDeltaTime();
		*m_animationTime = fmod(*m_animationTime, m_duration);
	}

	void AssimpAnimation::CalculateBoneTransform(const Ptr<Node> node, glm::mat4 globalTransformation)
	{
		globalTransformation *= node->GetTransform(*m_animationTime);
		if (node->AnyBones())
		{
			m_boneTransforms[node->GetBoneId()] = globalTransformation * node->GetBoneOffset();
		}

		for (uint32_t i = 0; i < node->GetNumOfChildNodes(); i++)
		{
			CalculateBoneTransform(node->GetChildNode(i), globalTransformation);
		}
	}
}
