#pragma once
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "Engine/Util/Vector3DHash.h"

namespace Engine
{
	enum class CollisionType
	{
		GjkEpa3D = 1,
	};

	struct ShapeInfo
	{
		glm::dvec3 position;
		glm::dmat4 orientation;
		glm::dmat4 scale;
		BoundingValue boundingValue;

		ShapeInfo() = default;
		ShapeInfo(const Transform& transform, const BoundingValue& boundingValue)
			: boundingValue(boundingValue)
		{
			position = transform.translation;
			orientation = glm::toMat4(glm::quat(transform.rotation));
			scale = glm::scale(glm::mat4(1.0f), transform.scale * boundingValue.extents);
		}
	};

	struct CollisionInfo
	{
		bool isCollided;
		double penetrationDepth;
		glm::dvec3 collisionNormal;

		// Test-only
		double separation;
		uint32_t iterations;

		CollisionInfo()
			: isCollided(false)
			, penetrationDepth(0.0)
			, collisionNormal()
			, separation(0.0)
			, iterations(0)
		{
		}
	};

	class Collision
	{
	public:
		virtual const CollisionInfo Detect(const ShapeInfo& shapeA, const ShapeInfo& shapeB) = 0;

		static Uniq<Collision> Create(CollisionType type);
	};
}
