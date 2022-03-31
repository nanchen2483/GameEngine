#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Engine
{
	class IModel
	{
	public:
		virtual std::string& GetPath() = 0;
		virtual void UpdateAnimation(float deltaTime) = 0;
		virtual std::vector<glm::mat4> GetPoseTransforms() = 0;
		virtual void Draw() = 0;
	};
}
