#pragma once
#include "Engine/Physics/Collision/ICollision.h"
#include "Engine/Physics/Collision/CollisionType.h"

namespace Engine
{
	class ENGINE_TEST_API CollisionFactory
	{
	public:
		static Uniq<ICollision> Create(CollisionType type);
	};
}
