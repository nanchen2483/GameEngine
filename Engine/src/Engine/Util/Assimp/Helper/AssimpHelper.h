#pragma once
#include "Engine/Util/Dictionary.h"
#include "../AssimpNode.h"

namespace Engine
{
	class AssimpHelper
	{
	public:
		static Ptr<Node> ConvertToAssimpNode(const aiAnimation* animation, const aiNode* rootNode, Dictionary<std::string, glm::mat4> boneOffsets)
		{
			auto boneMap = CreateBoneMap(animation, boneOffsets);
			return CreateNodes(rootNode, boneMap);
		}

	private:
		static std::map<std::string, AssimpBone> CreateBoneMap(const aiAnimation* animation, Dictionary<std::string, glm::mat4> boneOffsets)
		{
			std::map<std::string, AssimpBone> boneMap;
			for (uint32_t i = 0; i < animation->mNumChannels; i++)
			{
				const aiNodeAnim* channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.data;
				const auto& boneInfo = boneOffsets.SafeAdd(boneName, glm::mat4(1.0));
				boneMap[boneName] = AssimpBone(boneInfo.id, boneInfo.value, channel);
			}

			return boneMap;
		}

		static Ptr<Node> CreateNodes(const aiNode* node, std::map<std::string, AssimpBone>& boneMap)
		{
			Ptr<Node> newNode = CreatePtr<AssimpNode>(node, boneMap[node->mName.data]);
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				newNode->AddChildNode(CreateNodes(node->mChildren[i], boneMap));
			}

			return newNode;
		}
	};
}
