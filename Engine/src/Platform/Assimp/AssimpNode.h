#pragma once
#include "Platform/Assimp/AssimpBone.h"

namespace Engine
{
	class AssimpNode
	{
	public:
		AssimpNode() = default;
		AssimpNode(const aiNode* node, const AssimpBone bone);

		glm::mat4 GetTransform(float animationTime);
		bool HasBone() { return m_hasBone; }
		uint32_t GetBoneId();
		glm::mat4 GetBoneOffset();
		void AddChildNode(const AssimpNode childNode);
		AssimpNode GetChildNode(const uint32_t index);
		uint32_t GetNumOfChildNodes() { return m_numOfchildren; }
	private:
		glm::mat4 m_transformation;
		AssimpBone m_bone;
		bool m_hasBone;
		uint32_t m_numOfchildren;
		std::vector<AssimpNode> m_children;
	};
}
