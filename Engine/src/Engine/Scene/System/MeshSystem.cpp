#include "enginepch.h"
#include "MeshSystem.h"
#include "Engine/Core/System/System.h"

namespace Engine
{
	void MeshSystem::OnUpdate(MeshComponent& meshComponent, Transform& transform, PhysicsComponent physics, const Frustum& frustum, const Ptr<Terrain>& terrain)
	{
		if (!meshComponent.meshes.empty())
		{
			if (!physics.isStatic)
			{
				physics.fallingTime += System::GetDeltaTime();
				transform.translation.y -= System::GetFreeFallVelocity(physics.fallingTime);
			}

			if (terrain != nullptr)
			{
				transform.translation.y = terrain->GetHeight(transform.translation.x, transform.translation.z);
			}

			if (physics.boundingBox != nullptr)
			{
				meshComponent.isOnViewFrustum = physics.boundingBox->IsOnFrustum(frustum, transform);
			}
		}
	}
}
