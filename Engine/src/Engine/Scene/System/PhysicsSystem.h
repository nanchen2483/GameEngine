#pragma once
#include "ISystem.h"
#include "Engine/Core/Base.h"
#include "Engine/Physics/BoundingBox/Debug/BoundingBoxDebug.h"
#include "Engine/Physics/Collision/ICollision.h"
#include "Engine/Scene/Component/PhysicsComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"

#include <entt/entt.hpp>

namespace Engine
{
	class PhysicsSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry);
		static void DrawBoudingBox(entt::registry& registry);
	private:
		PhysicsSystem();
		static void Update(Transform& transformA, Transform& transformB, PhysicsComponent* physicsA, PhysicsComponent* physicsB);
		static PhysicsSystem& GetInstance();

		Uniq<BoundingBoxDebug> m_debug;
		Uniq<ICollision> m_collision;
	};
}
