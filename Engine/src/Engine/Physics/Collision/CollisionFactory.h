#pragma once
#include "ICollision.h"
#include "CollisionType.h"

namespace Engine
{
	class ENGINE_TEST_API CollisionFactory
	{
	public:
		static Uniq<ICollision> Create(CollisionType type);
	};
}
