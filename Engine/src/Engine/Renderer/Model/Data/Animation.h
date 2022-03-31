#pragma once
#include "Bone.h"

#include <vector>

namespace Engine::ModelData
{
	class Animation
	{
	public:
		Animation(aiAnimation* animation);
		Ptr<Bone> GetBoneByName(const std::string& name);
		float GetAnimationTime(float deltaTime);

	private:
		void SetupBones(const aiAnimation* animation);

		float m_currentTime;
		float m_duration;
		int m_ticksPerSecond;
		std::vector<Ptr<Bone>> m_bones;
	};
}
