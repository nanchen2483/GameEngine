#include "enginepch.h"
#include "ModelSystem.h"
#include "Engine/Renderer/Model/ModelLibrary.h"
#include "Engine/Scene/System/AnimationSystem.h"

namespace Engine
{
	void ModelSystem::OnUpdate(MeshComponent& meshComponent, Ptr<BoundingBox> boundingBox, Transform& modelTransform, const Frustum& frustum, const Ptr<Terrain>& terrain)
	{
		if (!meshComponent.meshes.empty())
		{
			if (terrain != nullptr)
			{
				modelTransform.translation.y = terrain->GetHeight(modelTransform.translation.x, modelTransform.translation.z);
			}

			meshComponent.isOnViewFrustum = boundingBox->IsOnFrustum(frustum, modelTransform);
		}
	}
}
