#pragma once
#include "Engine/Physics/BoundingBox/BoundingBox.h"

namespace Engine
{
	enum class CollisionType
	{
		GJK_EPA_3D = 1,
	};

	class Collision
	{
	public:
		~Collision() = default;

		virtual bool Detect(Transform transformA, Transform transformB, BoundingValue boudingValueA, BoundingValue boudingValueB) = 0;
		virtual float GetDistance() const = 0;
		virtual glm::vec3 GetDirectionFromAToB() const = 0;

		static Uniq<Collision> Create(CollisionType type);
	};
}
