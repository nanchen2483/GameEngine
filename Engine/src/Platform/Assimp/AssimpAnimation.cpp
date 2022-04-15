#include "enginepch.h"
#include "AssimpAnimation.h"

namespace Engine
{
	AssimpAnimation::AssimpAnimation(const aiAnimation* animation, const aiNode* rootNode, Dictionary<std::string, glm::mat4> boneOffsetDict)
		: m_currentTime(0.0f), m_duration(animation->mDuration), m_ticksPerSecond(animation->mTicksPerSecond), m_name(animation->mName.data)
	{
		SetupBones(animation, boneOffsetDict);
		SetupNodes(m_rootNode, rootNode);

		m_transforms.reserve(boneOffsetDict.Size());
		for (uint32_t i = 0; i < boneOffsetDict.Size(); i++)
		{
			m_transforms.push_back(glm::mat4(1.0f));
		}
	}

	void AssimpAnimation::SetupBones(const aiAnimation* animation, Dictionary<std::string, glm::mat4> boneOffsetDict)
	{
		for (uint32_t i = 0; i < animation->mNumChannels; i++)
		{
			const aiNodeAnim* channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;
			const auto& boneInfo = boneOffsetDict.AddIfNew(boneName, glm::mat4());
			m_boneMap[boneName] = AssimpBone(boneInfo.id, boneInfo.value, channel);
		}
	}

	void AssimpAnimation::SetupNodes(Node& dest, const aiNode* node)
	{
		dest.bone = m_boneMap[node->mName.data];
		dest.transformation = AssimpUtil::ToGlm(node->mTransformation);
		dest.numOfchildren = node->mNumChildren;
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			Node newNode;
			SetupNodes(newNode, node->mChildren[i]);
			dest.children.push_back(newNode);
		}
	}

	std::vector<glm::mat4> AssimpAnimation::GetBoneTransform(float deltaTime)
	{
		CalculateAnimationTime(deltaTime);
		CalculateBoneTransform(m_rootNode, glm::mat4(1.0f));

		return m_transforms;
	}

	void AssimpAnimation::CalculateAnimationTime(float deltaTime)
	{
		m_currentTime += m_ticksPerSecond * deltaTime;
		m_currentTime = fmod(m_currentTime, m_duration);
	}

	void AssimpAnimation::CalculateBoneTransform(const Node& node, glm::mat4 globalTransformation)
	{
		AssimpBone bone = node.bone;
		if (bone)
		{
			int boneId = bone.GetBoneId();
			globalTransformation *= bone.GetLocalTransform(m_currentTime);
			m_transforms[boneId] = globalTransformation * bone.GetBoneOffset();
		}
		else
		{
			globalTransformation *= node.transformation;
		}

		for (uint32_t i = 0; i < node.numOfchildren; i++)
		{
			CalculateBoneTransform(node.children[i], globalTransformation);
		}
	}
}
