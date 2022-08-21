#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/BoundingVolume/BoundingVolume.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <filesystem>
#include <glm/glm.hpp>
#include <vector>

namespace Engine
{
	struct AnimationInfo
	{
		uint32_t id;
		std::string displayName;
		Ptr<float> animationTime;
		float duration;
		float ticksPerSecond;
	};

	class Model
	{
	public:
		virtual std::filesystem::path GetFilePath() = 0;
		virtual bool HasAnimations() = 0;
		virtual std::vector<glm::mat4> GetBoneTransforms() const = 0;
		virtual BoundingValue GetBoundingValue() const = 0;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
		virtual void OnUpdate(float deltaTime) = 0;

		virtual const std::vector<AnimationInfo> GetAnimations() const = 0;
		virtual const AnimationInfo GetSelectedAnimation() const = 0;
		virtual void SetSelectedAnimation(const AnimationInfo animation) = 0;
		virtual void Draw() = 0;

		static Ptr<Model> Create(std::string path, bool gamma = false);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId, Ptr<TextureMap> textureMap);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId, Ptr<TextureMap> textureMap, Ptr<float> progression);
	};
}
