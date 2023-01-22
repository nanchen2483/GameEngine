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

	void CollisionSystem::OnUpdate(Transform& transformA, Transform& transformB, PhysicsComponent *physicsA, PhysicsComponent *physicsB)
	{
		if (physicsA == nullptr || physicsB == nullptr)
		{
			return;
		}

		GetInstance()->m_collision->Detect(
			transformA,
			transformB,
			physicsA->boundingBox->GetBoundingValue(),
			physicsB->boundingBox->GetBoundingValue());
		if (GetInstance()->m_collision->IsCollided())
		{
			float weightA = physicsA->weight;
			float weightB = physicsB->weight;
			float weightARatio = weightB / (weightA + weightB);
			float weightBRatio = weightA / (weightA + weightB);
			glm::vec3 distanceA = GetInstance()->m_collision->GetDirectionFromAToB() * weightARatio;
			glm::vec3 distanceB = GetInstance()->m_collision->GetDirectionFromAToB() * weightBRatio;
			transformA.translation += distanceA;
			transformB.translation -= distanceB;
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
