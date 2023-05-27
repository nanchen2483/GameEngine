#include "enginepch.h"
#include "CollisionFactory.h"

#include "Engine/Physics/Collision/GJKEPA3D/GJKEPA3D.h"

namespace Engine
{
	Uniq<ICollision> CollisionFactory::Create(CollisionType type)
	{
		switch (type)
		{
		case Engine::CollisionType::GjkEpa3D:
			return CreateUniq<GJKEPA3D>();
		default:
			ENGINE_CORE_ASSERT(false, "Unknow Collision Detection type");
			return nullptr;
		}
	}
}