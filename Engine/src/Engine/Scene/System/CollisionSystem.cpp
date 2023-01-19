#include "enginepch.h"
#include "CollisionSystem.h"

namespace Engine
{
	CollisionSystem* CollisionSystem::s_instance = nullptr;
	CollisionSystem::CollisionSystem()
	{
		m_collision = Collision::Create(CollisionType::GJK_EPA_3D);
		m_debug = CreateUniq<BoundingBoxDebug>();
	}

	void CollisionSystem::OnUpdate(Transform& transformA, Transform& transformB, Ptr<BoundingBox> boundingBoxA, Ptr<BoundingBox> boundingBoxB)
	{
		if (boundingBoxA == nullptr || boundingBoxB == nullptr)
		{
			return;
		}

		GetInstance()->m_collision->Detect(
			transformA,
			transformB,
			boundingBoxA->GetBoundingValue(),
			boundingBoxB->GetBoundingValue());
		if (GetInstance()->m_collision->IsCollided())
		{
			glm::vec3 halfDistance = GetInstance()->m_collision->GetDirectionFromAToB() / 2.0f;
			transformA.translation += halfDistance;
			transformB.translation -= halfDistance;
		}
	}

	void CollisionSystem::DrawBoudingBox(Transform& transform, Ptr<BoundingBox> boundingBox)
	{
		GetInstance()->m_debug->Draw(transform, boundingBox->GetBoundingValue());
	}

	CollisionSystem* CollisionSystem::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new CollisionSystem();
		}

		return s_instance;
	}
}
