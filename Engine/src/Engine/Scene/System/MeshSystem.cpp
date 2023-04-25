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
		registry.view<TransformComponent, MeshComponent, PhysicsComponent>()
			.each([&](TransformComponent& transformComponent, MeshComponent& meshComponent, PhysicsComponent& physicsComponent)
				{
					if (!meshComponent.meshes.empty())
					{
						// TODO: optimize gravity and terrain collision
						//if (!physics.isStatic)
						//{
						//	physics.fallingTime += System::GetDeltaTime();
						//	transform.translation.y -= System::GetFreeFallVelocity(physics.fallingTime);
						//}

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
