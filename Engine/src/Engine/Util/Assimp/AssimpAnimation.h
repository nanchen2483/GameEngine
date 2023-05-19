#pragma once
#include "AssimpNode.h"
#include "Engine/Renderer/Model/Animation/IAnimation.h"

namespace Engine
{
	class AssimpAnimation : public IAnimation
	{
	public:
		AssimpAnimation(const aiAnimation* animation, const Ptr<INode> rootNode);
		
		virtual const std::string GetName() const override { return m_name; }
		virtual const Ptr<float> GetTime() const override { return m_animationTime; }
		virtual const float GetDuration() const override { return m_duration; }
		virtual const float GetTicketPerSecond() const override { return m_ticksPerSecond; }
		virtual const Ptr<INode> GetRootNode() const { return m_rootNode; }
		virtual std::vector<glm::mat4> GetBoneTransforms() const override { return m_boneTransforms; };

		virtual void UpdateBoneTransforms(uint32_t boneId, glm::mat4 updatedTransform) override;
	private:
		const std::string m_name;
		Ptr<float> m_animationTime;
		const float m_duration;
		const float m_ticksPerSecond;

		const Ptr<INode> m_rootNode;
		std::vector<glm::mat4> m_boneTransforms;
	};
}
