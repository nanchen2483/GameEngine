#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Model/INode.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Engine
{
	class IAnimation
	{
	public:
		virtual const std::string GetName() const = 0;
		virtual const Ptr<float> GetTime() const = 0;
		virtual const float GetDuration() const = 0;
		virtual const float GetTicketPerSecond() const = 0;
		virtual const Ptr<INode> GetRootNode() const = 0;
		virtual std::vector<glm::mat4> GetBoneTransforms() const = 0;
		virtual void UpdateBoneTransforms(uint32_t boneId, glm::mat4 updatedTransform) = 0;
	};
}
