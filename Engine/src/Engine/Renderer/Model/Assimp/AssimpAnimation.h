#pragma once
#include "AssimpNode.h"

#include <vector>

namespace Engine
{
	class AssimpAnimation
	{
	public:
		AssimpAnimation(const aiAnimation* animation, const AssimpNode rootNode);
		
		void UpdateBoneTransforms();
		std::vector<glm::mat4> GetBoneTransforms() const { return m_boneTransforms; };
		const std::string GetName() const { return m_name; }
		const Ptr<float> GetTime() const { return m_animationTime; }
		const float GetDuration() const { return m_duration; }
		const float GetTicketPerSecond() const { return m_ticksPerSecond; }
	private:
		void CalculateAnimationTime();
		void CalculateBoneTransform(const AssimpNode& node, glm::mat4 globalTransformation);
		
		const std::string m_name;
		Ptr<float> m_animationTime;
		const float m_duration;
		const float m_ticksPerSecond;

		const AssimpNode m_rootNode;
		std::vector<glm::mat4> m_boneTransforms;
	};
}
