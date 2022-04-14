#pragma once

#include "AssimpBone.h"
#include <vector>
#include <map>

namespace Engine
{
	namespace AssimpData
	{
		struct Node
		{
			glm::mat4 transformation = glm::mat4();
			Ptr<AssimpBone> bone = nullptr;
			int numOfchildren = 0;
			std::vector<Node> children;
		};

		struct BoneOffset
		{
			uint32_t id;
			glm::mat4 offset;
		};
	}

	class AssimpAnimation
	{
	public:
		AssimpAnimation(const aiScene* scene);
		const uint32_t GetBoneId(const std::string& name);
		void UpdateBoneTransform(float deltaTime);
		std::vector<glm::mat4> GetBoneTransforms() { return m_transforms; }
	private:
		void SetupBones(const aiScene* scene);
		void SetupNodes(AssimpData::Node& dest, const aiNode* node);
		
		void CalculateAnimationTime(float deltaTime);
		void CalculateBoneTransform(const AssimpData::Node& node, glm::mat4 globalTransformation);
		
		float m_currentTime;
		const float m_duration;
		const float m_ticksPerSecond;
		std::map<std::string, Ptr<AssimpBone>> m_boneMap;
		AssimpData::Node m_rootNode;
		std::vector<glm::mat4> m_transforms;
	};
}
