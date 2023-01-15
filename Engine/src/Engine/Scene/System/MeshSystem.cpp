#include "enginepch.h"
#include "MeshSystem.h"

namespace Engine
{
	void MeshSystem::OnUpdate(MeshComponent& meshComponent, Ptr<BoundingBox> boundingBox, Transform& meshTransform, const Frustum& frustum, const Ptr<Terrain>& terrain)
	{
		if (!meshComponent.meshes.empty())
		{
			if (terrain != nullptr)
			{
				meshTransform.translation.y = terrain->GetHeight(meshTransform.translation.x, meshTransform.translation.z);
			}

			meshComponent.isOnViewFrustum = boundingBox->IsOnFrustum(frustum, meshTransform);
		}
	}
}
