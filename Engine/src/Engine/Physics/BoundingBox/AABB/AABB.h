#pragma once
#include "Engine/Physics/BoundingBox/IBoundingBox.h"

namespace Engine
{
	class AABB : public IBoundingBox
	{
	public:
		AABB(const glm::vec3& min, const glm::vec3& max);
		AABB(const glm::vec3& center, const float extentX, const float extentY, const float extentZ);

		virtual const BoundingValue& GetBoundingValue() const override { return m_boundingValue; }
		virtual void SetBoundingValue(const BoundingValue& value) override;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const override;
		virtual bool IsOnForwardPlane(const Plane& plan) const override;
		virtual glm::vec3 GetSupportPoint(glm::vec3 direction) const override;
	private:
		BoundingValue m_boundingValue;
	};
}
