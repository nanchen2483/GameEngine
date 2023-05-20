#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "BoundingValue.h"

#include <glm/glm.hpp>

namespace Engine
{
	class ENGINE_TEST_API IBoundingBox
	{
	public:
		~IBoundingBox() = default;

		virtual const BoundingValue& GetBoundingValue() const = 0;
		virtual void SetBoundingValue(const BoundingValue& value) = 0;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
		virtual bool IsOnForwardPlane(const Plane& plan) const = 0;
		virtual glm::vec3 GetSupportPoint(glm::vec3 direction) const = 0;
	};
}
