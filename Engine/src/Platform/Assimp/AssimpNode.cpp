#include "enginepch.h"
#include "AssimpNode.h"

namespace Engine
{
	AssimpNode::AssimpNode(const aiNode* node, const AssimpBone bone)
		:	m_bone(bone),
			m_hasBone(bone),
			m_transformation(AssimpUtil::ToGlm(node->mTransformation)),
			m_numOfchildren(node->mNumChildren)
	{
	}

	glm::mat4 AssimpNode::GetTransform(float animationTime)
	{
		if (m_hasBone)
		{
			return m_bone.GetLocalTransform(animationTime);
		}
		else
		{
			return m_transformation;
		}
	}

	uint32_t AssimpNode::GetBoneId()
	{
		if (m_hasBone)
		{
			return m_bone.GetId();
		}
		else
		{
			ENGINE_CORE_ASSERT(false, "This node contains no bones");
		}
	}

	glm::mat4 AssimpNode::GetBoneOffset()
	{
		if (m_hasBone)
		{
			return m_bone.GetOffset();
		}
		else
		{
			ENGINE_CORE_ASSERT(false, "This node contains no bones");
		}
	}
	
	void AssimpNode::AddChildNode(const AssimpNode childNode)
	{
		m_children.push_back(childNode);
	}
	
	AssimpNode AssimpNode::GetChildNode(const uint32_t index)
	{
		ENGINE_CORE_ASSERT(m_numOfchildren > index, "Invalid index");
		return m_children[index];
	}
}
