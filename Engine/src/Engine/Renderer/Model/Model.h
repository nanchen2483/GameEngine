#pragma once
#include "Mesh.h"
#include "Engine/Math/Transform.h"
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <filesystem>

namespace Engine
{
	struct AnimationInfo
	{
		uint32_t id = -1;
		std::string displayName;
		Ptr<float> animationTime = nullptr;
		float duration = 0.0f;
		float ticksPerSecond = 0.0f;
	};

	class Model : public IUniqueObject
	{
	public:
		virtual std::vector<Ptr<Mesh>> GetMeshes() const = 0;
		virtual std::filesystem::path GetFilePath() = 0;
		virtual bool HasAnimations() = 0;
		virtual std::vector<glm::mat4> GetBoneTransforms() const = 0;
		virtual BoundingValue GetBoundingValue() const = 0;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
		virtual void OnUpdate() = 0;

		virtual const std::vector<AnimationInfo> GetAnimations() const = 0;
		virtual const AnimationInfo GetSelectedAnimation() const = 0;
		virtual void SetSelectedAnimation(const AnimationInfo animation) = 0;
	private:
		static Ptr<Model> Create(std::string path, bool gamma = false);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId, Ptr<float> progression);

		friend class ModelLibrary;
	};
}
