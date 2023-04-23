#pragma once
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "Engine/Util/Vector3DHash.h"

namespace Engine
{
	enum class CollisionType
	{
		GJK_EPA_3D = 1,
	};

	struct ShapeInfo
	{
		glm::dvec3 position;
		glm::dmat4 orientation;
		glm::dmat4 scale;
		BoundingValue boundingValue;
		std::unordered_map<glm::dvec3, glm::dvec3, Vector3DHash> pointMap;

		ShapeInfo() = default;
		ShapeInfo(const Transform& transform, const BoundingValue& boundingValue)
			: boundingValue(boundingValue), pointMap({})
		{
			position = transform.translation;
			orientation = glm::toMat4(glm::quat(transform.rotation));
			scale = glm::scale(glm::mat4(1.0f), transform.scale * boundingValue.extents);
		}
	};

	struct CollisionInfo
	{
		bool isCollided;
		double separation;
		glm::dvec3 collisionNormal;

		// Test-only
		uint32_t iterations;

		CollisionInfo()
			: isCollided(false)
			, separation(0.0)
			, collisionNormal()
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
