#include "enginepch.h"
#include "AssimpAnimation.h"

#include "Engine/Core/System/System.h"

namespace Engine
{
	AssimpAnimation::AssimpAnimation(const aiAnimation* animation, const Ptr<INode> rootNode)
		:	m_name(animation->mName.data),
			m_animationTime(CreatePtr<float>(0.0f)),
			m_duration((float)animation->mDuration),
			m_ticksPerSecond((float)animation->mTicksPerSecond),
			m_rootNode(rootNode)
	{
		m_boneTransforms.reserve(200);
		for (uint32_t i = 0; i < 200; i++)
		{
			m_boneTransforms.push_back(glm::mat4(1.0f));
		}
	}

	void AssimpAnimation::UpdateBoneTransforms(uint32_t boneId, glm::mat4 updatedTransform)
	{
		m_boneTransforms[boneId] = updatedTransform;
	}
}
