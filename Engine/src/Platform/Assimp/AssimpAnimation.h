#pragma once

#include "AssimpBone.h"
#include "Engine/Util/Dictionary.h"

#include <vector>
#include <map>

namespace Engine
{
	class AssimpAnimation
	{
	public:
		AssimpAnimation(const aiAnimation* animation, const aiNode* rootNode, Dictionary<std::string, glm::mat4> boneOffsetDict);
		
		std::vector<glm::mat4> GetBoneTransform(float deltaTime);
		const std::string& GetName() const { return m_name; }
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

		void SetupBones(const aiAnimation* animation, Dictionary<std::string, glm::mat4> boneOffsetDict);
		void SetupNodes(Node& dest, const aiNode* node);
		void CalculateAnimationTime(float deltaTime);
		void CalculateBoneTransform(const Node& node, glm::mat4 globalTransformation);
		
		float m_currentTime;
		float m_duration;
		float m_ticksPerSecond;
		std::string m_name;

		std::map<std::string, AssimpBone> m_boneMap;
		Node m_rootNode;
		std::vector<glm::mat4> m_transforms;
	};
}
