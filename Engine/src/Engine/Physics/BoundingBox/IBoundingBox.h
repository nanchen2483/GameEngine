#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Camera/Frustum.h"

#include <functional>
#include <glm/glm.hpp>

namespace Engine
{
	struct BoundingValue
	{
		glm::vec3 center;
		glm::vec3 extents;

		std::function<glm::vec3(glm::vec3)> GetSupportPoint;
	};

	enum class BoundingBoxType
	{
		AABB = 1,
	};

	class ENGINE_TEST_API IBoundingBox
	{
	public:
		~IBoundingBox() = default;

		virtual BoundingValue GetBoundingValue() const = 0;
		virtual void SetBoundingValue(const BoundingValue& value) = 0;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const = 0;
		virtual bool IsOnForwardPlane(const Plane& plan) const = 0;
		virtual glm::vec3 GetSupportPoint(glm::vec3 direction) const = 0;

		static Ptr<IBoundingBox> Create(BoundingBoxType type, const glm::vec3& min, const glm::vec3& max);
	};
}
