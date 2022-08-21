#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Camera/Frustum.h"

#include <glm/glm.hpp>

namespace Engine
{
	struct BoundingValue
	{
		glm::vec3 center;
		glm::vec3 extents;

		std::function<glm::vec3(glm::vec3)> SupportMapping;
	};

	enum class BoundingVolumeType
	{
		AABB = 1,
	};

	class BoundingVolume
	{
	public:
		~BoundingVolume() = default;

		virtual BoundingValue GetBoundingValue() const = 0;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
		virtual bool IsOnForwardPlan(const Plan& plan) const = 0;

		static Uniq<BoundingVolume> Create(BoundingVolumeType type, const glm::vec3& min, const glm::vec3& max);
	};
}
