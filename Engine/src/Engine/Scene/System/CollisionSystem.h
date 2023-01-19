#pragma once
#include "ISystem.h"
#include "Engine/Core/Base.h"
#include "Engine/Physics/BoundingBox/Debug/BoundingBoxDebug.h"
#include "Engine/Physics/Collision/Collision.h"
#include "Engine/Scene/Component/TransformComponent.h"
namespace Engine
{
	class CollisionSystem : public ISystem
	{
	public:
		static void OnUpdate(Transform& transformA, Transform& transformB, Ptr<BoundingBox> boundingBoxA, Ptr<BoundingBox> boundingBoxB);
		static void DrawBoudingBox(Transform& transform, Ptr<BoundingBox> boundingBox);
	private:
		CollisionSystem();
		static CollisionSystem* GetInstance();

		Uniq<BoundingBoxDebug> m_debug;
		Uniq<Collision> m_collision;
		static CollisionSystem* s_instance;
	};
}
