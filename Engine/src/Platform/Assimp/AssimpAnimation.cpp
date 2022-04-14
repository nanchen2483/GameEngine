#include "enginepch.h"
#include "AssimpAnimation.h"

namespace Engine
{
	AssimpAnimation::AssimpAnimation(const aiScene* scene)
		: m_currentTime(0.0f),
		  DURATION(scene->mAnimations[0]->mDuration),
		  TICKS_PER_SECOND(scene->mAnimations[0]->mTicksPerSecond)
	{
		SetupBones(scene);
		SetupNodes(m_rootNode, scene->mRootNode);
	}

	void AssimpAnimation::SetupBones(const aiScene* scene)
	{
		// Get bone offset
		uint32_t boneId = 0;
		std::map<std::string, AssimpData::BoneOffset> boneOffsetMap;
		for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++)
		{
			const aiMesh* mesh = scene->mMeshes[meshIndex];
			for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
			{
				const aiBone* aiBone = mesh->mBones[boneIndex];
				const std::string boneName = aiBone->mName.data;
				if (boneOffsetMap.find(boneName) == boneOffsetMap.end())
				{
					const glm::mat4 offset = AssimpUtil::ToGlm(aiBone->mOffsetMatrix);
					boneOffsetMap[boneName] = AssimpData::BoneOffset{ boneId, offset };
					boneId++;
				}
			}
		}

		// Create AssimpBone map
		const aiAnimation* animation = scene->mAnimations[0];
		for (uint32_t channelIndex = 0; channelIndex < animation->mNumChannels; channelIndex++)
		{
			const aiNodeAnim* channel = animation->mChannels[channelIndex];
			std::string boneName = channel->mNodeName.data;
			if (boneOffsetMap.find(boneName) == boneOffsetMap.end())
			{
				m_boneMap[boneName] = CreatePtr<AssimpBone>(boneId, channel);
				boneId++;
			}
			else
			{
				const AssimpData::BoneOffset data = boneOffsetMap[boneName];
				m_boneMap[boneName] = CreatePtr<AssimpBone>(data.id, data.offset, channel);
			}
		}

		// Init transform matrix
		m_transforms.reserve(boneId);
		for (uint32_t i = 0; i < boneId; i++)
		{
			m_transforms.push_back(glm::mat4(1.0f));
		}
	}

	void AssimpAnimation::SetupNodes(AssimpData::Node& dest, const aiNode* node)
	{
		dest.bone = m_boneMap[node->mName.data];
		dest.transformation = AssimpUtil::ToGlm(node->mTransformation);
		dest.numOfchildren = node->mNumChildren;
		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			AssimpData::Node newNode;
			SetupNodes(newNode, node->mChildren[i]);
			dest.children.push_back(newNode);
		}
	}

	const uint32_t AssimpAnimation::GetBoneId(const std::string& boneName)
	{
		Ptr<AssimpBone> bone = m_boneMap[boneName];
		if (bone != nullptr)
		{
			return bone->GetBoneId();
		}

		ENGINE_CORE_ASSERT(false, "Bone not found with boneName: {0}", boneName);
	}

	void AssimpAnimation::UpdateBoneTransform(float deltaTime)
	{
		CalculateAnimationTime(deltaTime);
		CalculateBoneTransform(m_rootNode, glm::mat4(1.0f));
	}

	void AssimpAnimation::CalculateAnimationTime(float deltaTime)
	{
		m_currentTime += TICKS_PER_SECOND * deltaTime;
		m_currentTime = fmod(m_currentTime, DURATION);
	}

	void AssimpAnimation::CalculateBoneTransform(const AssimpData::Node& node, glm::mat4 globalTransformation)
	{
		Ptr<AssimpBone> bone = node.bone;
		if (bone != nullptr)
		{
			int boneId = bone->GetBoneId();
			globalTransformation *= bone->GetLocalTransform(m_currentTime);
			m_transforms[boneId] = globalTransformation * bone->GetBoneOffset();
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
