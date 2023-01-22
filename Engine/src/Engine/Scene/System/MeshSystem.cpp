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
				transform.translation.y -= System::GetFallVelocity();
			}

			if (terrain != nullptr)
			{
				transform.translation.y = terrain->GetHeight(transform.translation.x, transform.translation.z);
			}

			meshComponent.isOnViewFrustum = physics.boundingBox->IsOnFrustum(frustum, transform);
		}
	}
}
