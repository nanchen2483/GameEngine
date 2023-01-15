#pragma once
#include "ISystem.h"
#include "Engine/Core/Base.h"
#include "Engine/Physics/Collision/Collision.h"
#include "Engine/Scene/Component/TransformComponent.h"

namespace Engine
{
	class CollisionSystem : public ISystem
	{
	public:
		static void OnUpdate(Transform& transformA, Transform& transformB, Ptr<BoundingBox> boundingBoxA, Ptr<BoundingBox> boundingBoxB);
	private:
		static Uniq<Collision> s_collision;
	};
}
