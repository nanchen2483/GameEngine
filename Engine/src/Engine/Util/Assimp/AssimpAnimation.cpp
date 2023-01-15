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
}
