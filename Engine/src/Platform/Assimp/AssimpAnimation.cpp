#include "enginepch.h"
#include "AssimpAnimation.h"

namespace Engine
{
	AssimpAnimation::AssimpAnimation(aiAnimation* animation)
		: m_currentTime(0.0f), m_duration(animation->mDuration), m_ticksPerSecond(animation->mTicksPerSecond)
	{
		SetupBones(animation);
	}

	Ptr<AssimpBone> AssimpAnimation::GetBoneByName(const std::string& name)
	{
		std::vector<Ptr<AssimpBone>>::iterator iter = std::find_if(m_bones.begin(), m_bones.end(),
			[&](const Ptr<AssimpBone> bone)
			{
				return bone->GetBoneName() == name;
			});
		if (iter == m_bones.end())
		{
			return nullptr;
		}
		else
		{
			return *iter;
		}
	}

	float AssimpAnimation::GetAnimationTime(float deltaTime)
	{
		m_currentTime += m_ticksPerSecond * deltaTime;
		m_currentTime = fmod(m_currentTime, m_duration);

		return m_currentTime;
	}

	void AssimpAnimation::SetupBones(const aiAnimation* animation)
	{
		for (uint32_t i = 0; i < animation->mNumChannels; i++)
		{
			aiNodeAnim* channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;
			m_bones.push_back(CreatePtr<AssimpBone>(i, boneName, channel));
		}
	}
}
