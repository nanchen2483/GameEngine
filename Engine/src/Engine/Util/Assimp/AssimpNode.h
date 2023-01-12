#pragma once
#include "Engine/Renderer/Model/Node.h"
#include "AssimpBone.h"

namespace Engine
{
	class AssimpNode : public Node
	{
	public:
		AssimpNode() = default;
		AssimpNode(const aiNode* node, const AssimpBone bone);

		virtual const bool AnyBones() const { return m_anyBones; }
		virtual const uint32_t GetBoneId() const;
		virtual const glm::mat4 GetBoneOffset() const;
		virtual const glm::mat4 GetTransform(float animationTime) const;

		virtual const Ptr<Node> GetChildNode(const uint32_t index) const;
		virtual const uint32_t GetNumOfChildNodes() const { return m_numOfchildren; }
		virtual void AddChildNode(Ptr<Node> childNode) { m_children.push_back(childNode); }
	private:
		glm::mat4 m_transformation;
		AssimpBone m_bone;
		bool m_anyBones;
		uint32_t m_numOfchildren;
		std::vector<Ptr<Node>> m_children;
	};
}
