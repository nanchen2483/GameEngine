#pragma once
#include "Engine/Core/Base.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Engine
{
	class Animation
	{
	public:
		virtual std::vector<glm::mat4> GetBoneTransforms() const = 0;
		virtual const std::string GetName() const = 0;
		virtual const Ptr<float> GetTime() const = 0;
		virtual const float GetDuration() const = 0;
		virtual const float GetTicketPerSecond() const = 0;
	};
}
