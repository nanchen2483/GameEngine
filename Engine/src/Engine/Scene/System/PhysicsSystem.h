#pragma once
#include "ISystem.h"
#include "Engine/Core/Base.h"
#include "Engine/Physics/BoundingBox/Debug/BoundingBoxDebug.h"
#include "Engine/Physics/Collision/Collision.h"
#include "Engine/Scene/Component/PhysicsComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"

namespace Engine
{
	class PhysicsSystem : public ISystem
	{
	public:
		static void OnUpdate(Transform& transformA, Transform& transformB, PhysicsComponent *physicsA, PhysicsComponent *physicsB);
		static void DrawBoudingBox(Transform& transform, Ptr<BoundingBox> boundingBox);
	private:
		PhysicsSystem();
		static PhysicsSystem* GetInstance();

		Uniq<BoundingBoxDebug> m_debug;
		Uniq<Collision> m_collision;
		static PhysicsSystem* s_instance;
	};
}
