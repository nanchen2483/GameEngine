#pragma once
#include "Engine/Core/Base.h"

#include <glm/glm.hpp>

namespace Engine
{
	class Node
	{
	public:
		virtual const bool AnyBones() const = 0;
		virtual const uint32_t GetBoneId() const = 0;
		virtual const glm::mat4 GetBoneOffset() const = 0;
		virtual const glm::mat4 GetTransform(float animationTime) const = 0;
		virtual const Ptr<Node> GetChildNode(const uint32_t index) const = 0;
		virtual const uint32_t GetNumOfChildNodes() const = 0;
		virtual void AddChildNode(Ptr<Node> childNode) = 0;
	};
}
