#include "enginepch.h"
#include "MeshSystem.h"
#include "Engine/Core/System/System.h"
#include "Engine/Scene/Component/TransformComponent.h"
#include "Engine/Scene/Component/MeshComponent.h"
#include "Engine/Scene/Component/PhysicsComponent.h"

namespace Engine
{
	void MeshSystem::OnUpdate(entt::registry& registry, const Frustum& frustum, const Frustum& lightViewFrustum, const Ptr<Terrain>& terrain)
	{
		const float dampingFactor = 0.9f;
		float deltaTime = System::GetDeltaTime();
		registry.view<TransformComponent, MeshComponent, PhysicsComponent>()
			.each([&](TransformComponent& transformComponent, MeshComponent& meshComponent, PhysicsComponent& physicsComponent)
				{
					if (!meshComponent.meshes.empty())
					{
						// TODO: optimize gravity and terrain collision
						//if (!physicsComponent.isStatic)
						//{
						//	physicsComponent.fallingTime += System::GetDeltaTime();
						//	transformComponent.transform.translation.y -= System::GetFreeFallVelocity(physicsComponent.fallingTime);
						//}

						if (!meshComponent.isPlayer && transformComponent.transform.velocity != glm::vec3(0.0))
						{
							transformComponent.transform.translation += (transformComponent.transform.velocity * deltaTime);
							transformComponent.transform.velocity *= dampingFactor;
						}

						if (terrain != nullptr)
						{
							transformComponent.transform.translation.y = terrain->GetHeight(
								transformComponent.transform.translation.x,
								transformComponent.transform.translation.z);
						}

						if (physicsComponent.boundingBox != nullptr)
						{
							meshComponent.isOnViewFrustum = physicsComponent.boundingBox->IsOnFrustum(frustum, transformComponent.transform);
							meshComponent.isOnLightViewFrustum = physicsComponent.boundingBox->IsOnFrustum(lightViewFrustum, transformComponent.transform);
						}
					}
				});
	}
}
