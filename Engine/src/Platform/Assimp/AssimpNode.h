#pragma once
#include "AssimpAnimation.h"

namespace Engine
{
	struct AssimpNode
	{
		glm::mat4 transformation;
		Ptr<AssimpBone> bone;
		int numOfchildren;
		std::vector<Ptr<AssimpNode>> children;

		AssimpNode(const aiNode* node, const Ptr<AssimpAnimation> animation)
		{
			bone = animation->GetBoneByName(node->mName.data);
			transformation = AssimpUtil::ToGlm(node->mTransformation);
			numOfchildren = node->mNumChildren;
			for (int i = 0; i < node->mNumChildren; i++)
			{
				children.push_back(CreatePtr<AssimpNode>(node->mChildren[i], animation));
			}
		}
	};
}
