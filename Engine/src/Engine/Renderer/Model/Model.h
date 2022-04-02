#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace Engine
{
	class Model
	{
	public:
		virtual std::string& GetPath() = 0;
		virtual void UpdateAnimation(float deltaTime) = 0;
		virtual std::vector<glm::mat4> GetPoseTransforms() = 0;
		virtual void Draw() = 0;

		static Ptr<Model> Create(std::string path);
		static Ptr<Model> Create(std::string path, bool gamma);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId);
	};
}
