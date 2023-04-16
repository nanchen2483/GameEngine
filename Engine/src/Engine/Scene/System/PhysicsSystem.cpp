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

		const CollisionInfo info = GetInstance()->m_collision->Detect(shapeA, shapeB);
		if (info.isCollided)
		{
			// Calculate the relative velocity of the two objects
			glm::dvec3 relativeVelocity = transformB.velocity - transformA.velocity;

			// Calculate the relative velocity in the direction of the collision normal
			double velocityAlongNormal = glm::dot(relativeVelocity, info.collisionNormal);

			// Calculate the restitution (bounciness) of the collision
			double e = std::min(physicsA->restitution, physicsB->restitution);

			// Calculate the impulse scalar
			double j = -(1.0 + e) * velocityAlongNormal / (1.0 / physicsA->mass + 1.0 / physicsB->mass);

			// Apply the impulse to the objects
			glm::dvec3 impulse = j * info.collisionNormal;
			transformA.velocity -= 1.0 / (double)physicsA->mass * impulse;
			transformB.velocity += 1.0 / (double)physicsB->mass * impulse;

			// Correct the positions of the objects to avoid overlap
			const double percent = 0.2; // percentage of overlap to correct
			const double slop = 0.01; // small value to avoid jitter
			glm::dvec3 correction = std::max(std::abs(info.separation) - slop, 0.0) / (1.0 / physicsA->mass + 1.0 / physicsB->mass) * percent * info.collisionNormal;
			transformA.translation += 1.0 / (double)physicsA->mass * correction;
			transformB.translation -= 1.0 / (double)physicsB->mass * correction;
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
