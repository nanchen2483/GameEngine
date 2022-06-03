#pragma once
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Scene/Component.h"

#include <glm/glm.hpp>

namespace Engine
{
	enum class BoundingVolumeType
	{
		Unknown = 0,
		AABB,
	};

	class BoundingVolume
	{
	public:
		~BoundingVolume() = default;

		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
		virtual bool IsOnForwardPlan(const Plan& plan) const = 0;

		static Uniq<BoundingVolume> Create(BoundingVolumeType type, const glm::vec3& min, const glm::vec3& max);
	};
}
