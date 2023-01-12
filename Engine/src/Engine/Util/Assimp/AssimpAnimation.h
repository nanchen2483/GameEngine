#pragma once
#include "AssimpNode.h"
#include "Engine/Renderer/Model/Animation.h"

namespace Engine
{
	class AssimpAnimation : public Animation
	{
	public:
		AssimpAnimation(const aiAnimation* animation, const Ptr<Node> rootNode);
		
		virtual const std::string GetName() const override { return m_name; }
		virtual const Ptr<float> GetTime() const override { return m_animationTime; }
		virtual const float GetDuration() const override { return m_duration; }
		virtual const float GetTicketPerSecond() const override { return m_ticksPerSecond; }
		virtual const Ptr<Node> GetRootNode() const { return m_rootNode; }
		virtual std::vector<glm::mat4> GetBoneTransforms() const override { return m_boneTransforms; };
		virtual void UpdateBoneTransforms(uint32_t boneId, glm::mat4 updatedTransform) override { m_boneTransforms[boneId] = updatedTransform; };

		void UpdateBoneTransforms();
	private:
		void CalculateAnimationTime();
		void CalculateBoneTransform(const Ptr<Node> node, glm::mat4 globalTransformation);
		
		const std::string m_name;
		Ptr<float> m_animationTime;
		const float m_duration;
		const float m_ticksPerSecond;

		const Ptr<Node> m_rootNode;
		std::vector<glm::mat4> m_boneTransforms;
	};
}
