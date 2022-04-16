#pragma once
#include "AssimpNode.h"
#include <vector>

namespace Engine
{
	class AssimpAnimation
	{
	public:
		AssimpAnimation() = default;
		AssimpAnimation(const aiAnimation* animation, const AssimpNode rootNode);
		
		void UpdateBoneTransforms(float deltaTime);
		std::vector<glm::mat4> GetBoneTransforms() const { return m_boneTransforms; };
		const std::string GetName() const { return m_name; }
		const Ptr<float> GetTime() const { return m_animationTime; }
		const float GetDuration() const { return m_duration; }
		const float GetTicketPerSecond() const { return m_ticksPerSecond; }
	private:
		void CalculateAnimationTime(float deltaTime);
		void CalculateBoneTransform(AssimpNode& node, glm::mat4 globalTransformation);
		
		std::string m_name;
		Ptr<float> m_animationTime;
		float m_duration;
		float m_ticksPerSecond;

		AssimpNode m_rootNode;
		std::vector<glm::mat4> m_boneTransforms;
	};
}
