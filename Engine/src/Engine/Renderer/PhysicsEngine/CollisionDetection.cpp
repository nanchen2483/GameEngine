#include "enginepch.h"
#include "CollisionDetection.h"
#include "Engine/Renderer/PhysicsEngine/GJKEPA3D.h"

namespace Engine
{
	Uniq<CollisionDetection> CollisionDetection::Create(CollisionDetectionType type)
	{
		switch (type)
		{
		case Engine::CollisionDetectionType::GJK_EPA_3D:
			return CreateUniq<GJKEPA3D>();
		default:
			ENGINE_CORE_ASSERT(false, "Unknow Collision Detection type");
			return nullptr;
		}
	}
}
