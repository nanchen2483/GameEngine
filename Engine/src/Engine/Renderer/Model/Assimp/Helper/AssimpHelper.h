#pragma once
#include "Engine/Util/Dictionary.h"
#include "../Assimp/AssimpNode.h"

namespace Engine
{
	class AssimpHelper
	{
	public:
		static AssimpNode ConvertToAssimpNode(const aiAnimation* animation, const aiNode* rootNode, const Dictionary<std::string, glm::mat4> boneOffsets)
		{
			auto boneMap = CreateBoneMap(animation, boneOffsets);
			return CreateNodes(rootNode, boneMap);
		}

	private:
		static std::map<std::string, AssimpBone> CreateBoneMap(const aiAnimation* animation, const Dictionary<std::string, glm::mat4> boneOffsets)
		{
			std::map<std::string, AssimpBone> boneMap;
			for (uint32_t i = 0; i < animation->mNumChannels; i++)
			{
				const aiNodeAnim* channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.data;
				const auto& boneInfo = boneOffsets.Get(boneName);
				boneMap[boneName] = AssimpBone(boneInfo.id, boneInfo.value, channel);
			}

			return boneMap;
		}

		static AssimpNode CreateNodes(const aiNode* node, std::map<std::string, AssimpBone>& boneMap)
		{
			AssimpNode newNode(node, boneMap[node->mName.data]);
			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				newNode.AddChildNode(CreateNodes(node->mChildren[i], boneMap));
			}

			return newNode;
		}
	};
}
