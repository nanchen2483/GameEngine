#pragma once
#include "CollisionInfo.h"
#include "ShapeInfo.h"

namespace Engine
{
	class ENGINE_TEST_API ICollision
	{
	public:
		virtual const CollisionInfo Detect(const ShapeInfo& shapeA, const ShapeInfo& shapeB) = 0;
	};
}
