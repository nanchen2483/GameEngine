#pragma once
#include "Engine/Renderer/BoundingVolume/BoundingVolume.h"

namespace Engine
{
	enum class CollisionDetectionType
	{
		GJK_EPA_3D = 1,
	};

	class CollisionDetection
	{
	public:
		~CollisionDetection() = default;

		virtual bool Detect(Transform transformA, Transform transformB, BoundingValue boudingValueA, BoundingValue boudingValueB) = 0;
		virtual float GetDistance() const = 0;
		virtual glm::vec3 GetDirectionFromAToB() const = 0;

		static Uniq<CollisionDetection> Create(CollisionDetectionType type);
	};
}
