#pragma once

#include "AssimpBone.h"

#include <vector>
#include <map>

namespace Engine
{
	class AssimpAnimation
	{
	public:
		AssimpAnimation(const aiScene* scene);
		std::vector<glm::mat4> GetBoneTransform(float deltaTime);
		const uint32_t GetBoneId(const std::string& name);
		float* GetTime() { return &m_currentTime; }
		const float GetDuration() const { return m_duration; }
	private:
		struct Node
		{
			glm::mat4 transformation = glm::mat4();
			AssimpBone bone = {};
			int numOfchildren = 0;
			std::vector<Node> children;
		};

		void SetupBones(const aiScene* scene);
		void SetupNodes(Node& dest, const aiNode* node);
		void CalculateAnimationTime(float deltaTime);
		void CalculateBoneTransform(const Node& node, glm::mat4 globalTransformation);
		
		float m_currentTime;
		const float m_duration;
		const float m_ticksPerSecond;
		std::map<std::string, AssimpBone> m_boneMap;
		Node m_rootNode;
		std::vector<glm::mat4> m_transforms;
	};
}
