#include "enginepch.h"
#include "AssimpNode.h"

namespace Engine
{
	AssimpNode::AssimpNode(const aiNode* node, const AssimpBone bone)
		:	m_bone(bone),
			m_anyBones(bone),
			m_transformation(AssimpUtil::ToGlm(node->mTransformation)),
			m_numOfchildren(node->mNumChildren)
	{
	}

	const glm::mat4 AssimpNode::GetTransform(float animationTime) const
	{
		if (m_anyBones)
		{
			return m_bone.GetLocalTransform(animationTime);
		}
		else
		{
			return m_transformation;
		}
	}

	const uint32_t AssimpNode::GetBoneId() const
	{
		if (m_anyBones)
		{
			return m_bone.GetId();
		}
		
		ENGINE_CORE_ASSERT(false, "This node contains no bones");
		return 0;
	}

	const glm::mat4 AssimpNode::GetBoneOffset() const
	{
		if (m_anyBones)
		{
			return m_bone.GetOffset();
		}
		
		ENGINE_CORE_ASSERT(false, "This node contains no bones");
		return {};
	}
	
	const Ptr<INode> AssimpNode::GetChildNode(const uint32_t index) const
	{
		ENGINE_CORE_ASSERT(m_numOfchildren > index, "Invalid index");
		return m_children[index];
	}
}
