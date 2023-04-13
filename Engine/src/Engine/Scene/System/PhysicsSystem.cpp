#include "enginepch.h"
#include "PhysicsSystem.h"

namespace Engine
{
	PhysicsSystem* PhysicsSystem::s_instance = nullptr;
	PhysicsSystem::PhysicsSystem()
	{
		m_collision = Collision::Create(CollisionType::GJK_EPA_3D);
		m_debug = CreateUniq<BoundingBoxDebug>();
	}

	void PhysicsSystem::OnUpdate(Transform& transformA, Transform& transformB, PhysicsComponent *physicsA, PhysicsComponent *physicsB)
	{
		if (physicsA == nullptr || physicsB == nullptr)
		{
			return;
		}

		if (physicsA->isStatic && physicsB->isStatic)
		{
			return;
		}

		ShapeInfo shapeA(transformA, physicsA->boundingBox->GetBoundingValue());
		ShapeInfo shapeB(transformB, physicsB->boundingBox->GetBoundingValue());

		GetInstance()->m_collision->Detect(shapeA, shapeB);
		if (GetInstance()->m_collision->IsCollided())
		{
			glm::vec3 distanceAtoB = GetInstance()->m_collision->GetDirectionFromAToB();
			if (physicsA->isStatic)
			{
				transformB.translation -= distanceAtoB;
			}
			else if (physicsB->isStatic)
			{
				transformA.translation += distanceAtoB;
			}
			else
			{
				float weightA = physicsA->weight;
				float weightB = physicsB->weight;
				float weightARatio = weightB / (weightA + weightB);
				float weightBRatio = weightA / (weightA + weightB);
				glm::vec3 distanceA = distanceAtoB * weightARatio;
				glm::vec3 distanceB = distanceAtoB * weightBRatio;
				transformA.translation += distanceA;
				transformB.translation -= distanceB;
			}

			physicsA->fallingTime = 0.0f;
			physicsB->fallingTime = 0.0f;
		}
	}

	void PhysicsSystem::DrawBoudingBox(Transform& transform, Ptr<BoundingBox> boundingBox)
	{
		GetInstance()->m_debug->Draw(transform, boundingBox->GetBoundingValue());
	}

	PhysicsSystem* PhysicsSystem::GetInstance()
	{
		if (!s_instance)
		{
			s_instance = new PhysicsSystem();
		}

		return s_instance;
	}
}
