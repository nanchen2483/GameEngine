#include "enginepch.h"
#include "PhysicsSystem.h"

namespace Engine
{
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
		const PhysicsSystem& instance = GetInstance();
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

		const CollisionInfo& info = instance.m_collision->Detect(shapeA, shapeB);
		if (info.isCollided)
		{
			// Calculate the relative velocity of the two objects
			glm::dvec3 relativeVelocity = transformB.velocity - transformA.velocity;

			// Calculate the relative velocity in the direction of the collision normal
			double velocityAlongNormal = glm::dot(relativeVelocity, info.collisionNormal);

			// Calculate the restitution (bounciness) of the collision
			double e = std::min(physicsA->restitution, physicsB->restitution);

			// Calculate the impulse scalar
			double invMassA = 1.0 / (double)physicsA->mass;
			double invMassB = 1.0 / (double)physicsB->mass;
			double invMassSum = invMassA + invMassB;
			double j = -(1.0 + e) * velocityAlongNormal / invMassSum;
			glm::dvec3 impulse = j * info.collisionNormal;

			// Correct the positions of the objects to avoid overlap
			const double percent = 1.0; // percentage of overlap to correct
			const double slop = 0.01; // small value to avoid jitter
			double absPenetrationDepth = std::abs(info.penetrationDepth);
			double penetrationSlop = std::max(absPenetrationDepth - slop, 0.0);
			double correctionMagnitude = (penetrationSlop / invMassSum) * percent;
			glm::dvec3 correction = correctionMagnitude * info.collisionNormal;
			if (physicsA->isStatic)
			{
				transformB.velocity += invMassSum * impulse;
				transformB.translation += invMassSum * correction;
				physicsB->fallingTime = 0.0f;
			}
			else if (physicsB->isStatic)
			{
				transformA.velocity -= invMassSum * impulse;
				transformA.translation -= invMassSum * correction;
				physicsA->fallingTime = 0.0f;
			}
			else
			{
				transformA.velocity -= invMassA * impulse;
				transformB.velocity += invMassB * impulse;
				physicsA->fallingTime = 0.0f;
				transformA.translation -= invMassA * correction;
				transformB.translation += invMassB * correction;
				physicsB->fallingTime = 0.0f;
			}
		}
	}

	void PhysicsSystem::DrawBoudingBox(entt::registry& registry)
	{
		const PhysicsSystem& instance = GetInstance();
		registry.view<TransformComponent, PhysicsComponent>()
			.each([&](TransformComponent& transform, PhysicsComponent& component)
				{
					instance.m_debug->Draw(transform, component.boundingBox->GetBoundingValue());
				});
	}

	PhysicsSystem& PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}
}
