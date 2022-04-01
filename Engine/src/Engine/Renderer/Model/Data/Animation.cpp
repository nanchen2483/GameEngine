#include "enginepch.h"
#include "Animation.h"

namespace Engine::ModelData
{
	Animation::Animation(aiAnimation* animation)
		: m_currentTime(0.0f), m_duration(animation->mDuration), m_ticksPerSecond(animation->mTicksPerSecond)
	{
		SetupBones(animation);
	}

	Ptr<Bone> Animation::GetBoneByName(const std::string& name)
	{
		std::vector<Ptr<Bone>>::iterator iter = std::find_if(m_bones.begin(), m_bones.end(),
			[&](const Ptr<Bone> bone)
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

	float Animation::GetAnimationTime(float deltaTime)
	{
		m_currentTime += m_ticksPerSecond * deltaTime;
		m_currentTime = fmod(m_currentTime, m_duration);

		return m_currentTime;
	}

	void Animation::SetupBones(const aiAnimation* animation)
	{
		for (uint32_t i = 0; i < animation->mNumChannels; i++)
		{
			aiNodeAnim* channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;
			m_bones.push_back(CreatePtr<Bone>(i, boneName, channel));
		}
	}
}
