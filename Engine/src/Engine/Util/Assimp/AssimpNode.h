#pragma once
#include "AssimpBone.h"

namespace Engine
{
	class AssimpNode
	{
	public:
		AssimpNode() = default;
		AssimpNode(const aiNode* node, const AssimpBone bone);

		const bool HasBone() const { return m_hasBone; }
		const uint32_t GetBoneId() const;
		const glm::mat4 GetBoneOffset() const;
		const glm::mat4 GetTransform(float animationTime) const;

		const AssimpNode& GetChildNode(const uint32_t index) const;
		const uint32_t GetNumOfChildNodes() const { return m_numOfchildren; }
		void AddChildNode(const AssimpNode childNode) { m_children.push_back(childNode); }
	private:
		glm::mat4 m_transformation;
		AssimpBone m_bone;
		bool m_hasBone;
		uint32_t m_numOfchildren;
		std::vector<AssimpNode> m_children;
	};
}
