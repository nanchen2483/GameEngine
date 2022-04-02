#pragma once

#include "AssimpBone.h"

#include <vector>

namespace Engine
{
	class AssimpAnimation
	{
	public:
		AssimpAnimation(aiAnimation* animation);
		Ptr<AssimpBone> GetBoneByName(const std::string& name);
		float GetAnimationTime(float deltaTime);

	private:
		void SetupBones(const aiAnimation* animation);

		float m_currentTime;
		float m_duration;
		int m_ticksPerSecond;
		std::vector<Ptr<AssimpBone>> m_bones;
	};
}
