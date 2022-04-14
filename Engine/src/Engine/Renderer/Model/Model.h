#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Engine/Renderer/Texture/Texture.h"

namespace Engine
{
	class Model
	{
	public:
		virtual std::string GetFilePath() = 0;
		virtual void UpdateAnimation(float deltaTime) = 0;
		virtual std::vector<glm::mat4> GetPoseTransforms() = 0;
		virtual bool HasAnimations() = 0;
		virtual void Draw() = 0;

		static Ptr<Model> Create(std::string path);
		static Ptr<Model> Create(std::string path, bool gamma);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId, TextureMap* textureMap);
	};
}
