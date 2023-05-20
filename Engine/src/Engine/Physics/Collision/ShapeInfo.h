#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Physics/BoundingBox/BoundingValue.h"

namespace Engine
{
	struct ShapeInfo
	{
		glm::dvec3 position{};
		glm::dmat4 orientation{};
		glm::dmat4 scale{};
		BoundingValue boundingValue{};

		ShapeInfo() = default;
		ShapeInfo(const Transform& transform, const BoundingValue& value)
		{
			position = transform.translation;
			orientation = glm::toMat4(glm::quat(transform.rotation));
			scale = glm::scale(glm::mat4(1.0f), transform.scale * value.extents);
			boundingValue = value;
		}
	};
}
