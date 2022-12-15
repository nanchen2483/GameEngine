#include "ModelSystem.h"

namespace Engine
{
	void ModelSystem::OnUpdate(ModelComponent& modelComponent, Transform& modelTransform, const Frustum& frustum, const Ptr<Terrain>& terrain)
	{
		if (modelComponent.model != nullptr)
		{
			if (terrain != nullptr)
			{
				modelTransform.translation.y = terrain->GetHeight(modelTransform.translation.x, modelTransform.translation.z);
			}

			modelComponent.isOnViewFrustum = modelComponent.model->IsOnFrustum(frustum, modelTransform);
			if (modelComponent.enableAnimation)
			{
				modelComponent.model->OnUpdate();
			}
		}
	}
}
