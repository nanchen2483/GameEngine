#pragma once
#include "BoundingVolume.h"

namespace Engine
{
	class AABB : public BoundingVolume
	{
	public:
		AABB(const glm::vec3& min, const glm::vec3& max);
		AABB(const glm::vec3& center, const float extentX, const float extentY, const float extentZ);

		virtual BoundingValue GetBoundingValue() const override { return m_boundingValue; }
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const override;
		virtual bool IsOnForwardPlan(const Plan& plan) const override;
	private:
		BoundingValue m_boundingValue;
	};
}
