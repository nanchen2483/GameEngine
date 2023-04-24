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

	void PhysicsSystem::OnUpdate(entt::registry& registry)
	{
		auto meshView = registry.view<TransformComponent, PhysicsComponent>();
		meshView.each([&](entt::entity thisEntity, TransformComponent& thisTransform, PhysicsComponent& thisPhysics)
			{
				meshView.each([&](entt::entity otherOntity, TransformComponent& otherTransform, PhysicsComponent& otherPhysics)
					{
						// Skip self and entities that have already been compared
						if (thisEntity < otherOntity)
						{
							Update(thisTransform, otherTransform, &thisPhysics, &otherPhysics);
						}
					});
			});
	}

	void PhysicsSystem::Update(Transform& transformA, Transform& transformB, PhysicsComponent* physicsA, PhysicsComponent* physicsB)
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

		const CollisionInfo& info = GetInstance()->m_collision->Detect(shapeA, shapeB);
		if (info.isCollided)
		{
			// Calculate the relative velocity of the two objects
			glm::dvec3 relativeVelocity = transformB.velocity - transformA.velocity;

			// Calculate the relative velocity in the direction of the collision normal
			double velocityAlongNormal = glm::dot(relativeVelocity, info.collisionNormal);

			// Calculate the restitution (bounciness) of the collision
			double e = std::min(physicsA->restitution, physicsB->restitution);
			double ePlusOne = 1.0 + e;

			// Calculate the impulse scalar
			double invMassA = 1.0 / (double)physicsA->mass;
			double invMassB = 1.0 / (double)physicsB->mass;
			double invMassSum = invMassA + invMassB;
			double j = -ePlusOne * velocityAlongNormal / invMassSum;

			// Apply the impulse to the objects
			glm::dvec3 impulse = j * info.collisionNormal;
			transformA.velocity += invMassA * impulse;
			transformB.velocity -= invMassB * impulse;

			// Correct the positions of the objects to avoid overlap
			const double percent = 0.2; // percentage of overlap to correct
			const double slop = 0.01; // small value to avoid jitter
			double absPenetrationDepth = std::abs(info.penetrationDepth);
			double penetrationSlop = std::max(absPenetrationDepth - slop, 0.0);
			double correctionMagnitude = (penetrationSlop / invMassSum) * percent;
			glm::dvec3 correction = correctionMagnitude * info.collisionNormal;
			transformA.translation -= invMassA * correction;
			transformB.translation += invMassB * correction;
		}
	}

	void PhysicsSystem::DrawBoudingBox(entt::registry& registry)
	{
		registry.view<TransformComponent, PhysicsComponent>()
			.each([=](TransformComponent& transform, PhysicsComponent& component)
				{
					GetInstance()->m_debug->Draw(transform, component.boundingBox->GetBoundingValue());
				});
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
