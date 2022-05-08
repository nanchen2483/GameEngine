#pragma once

#include "BoundingVolume.h"

namespace Engine
{
	class AABB : public BoundingVolume
	{
	public:
		AABB(const glm::vec3& min, const glm::vec3& max);
		AABB(const glm::vec3& center, const float extentX, const float extentY, const float extentZ);

		virtual bool IsOnFrustum(const Frustum& cameraFrustum, const Transform& transform) const override;
		virtual bool IsOnForwardPlan(const Plan& plan) const override;
	private:
		glm::vec3 m_center;
		glm::vec3 m_extents;
	};
}
