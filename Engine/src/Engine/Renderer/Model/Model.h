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
		virtual bool HasAnimations() = 0;
		virtual std::vector<glm::mat4> GetBoneTransforms(float deltaTime) = 0;
		virtual float* GetAnimationTime() = 0;
		virtual const float GetAnimationDuration() = 0;
		virtual void Draw() = 0;

		static Ptr<Model> Create(std::string path);
		static Ptr<Model> Create(std::string path, bool gamma);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId, TextureMap* textureMap);
	};
}
