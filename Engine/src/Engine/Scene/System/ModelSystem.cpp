#include "enginepch.h"
#include "ModelSystem.h"
#include "Engine/Renderer/Model/ModelLibrary.h"

namespace Engine
{
	void ModelSystem::Load(ModelComponent* modelComponent, const std::string& filepath, uint32_t entityId)
	{
		std::thread([=]()
			{
				modelComponent->loading = true;
				modelComponent->model = ModelLibrary::GetInstance()->Load(filepath, entityId);
				modelComponent->meshes = modelComponent->model->GetMeshes();
				modelComponent->loading = false;
				*modelComponent->progression = 0.0f;
			}).detach();
	}

	void ModelSystem::OnUpdate(ModelComponent& modelComponent, Transform& modelTransform, const Frustum& frustum, const Ptr<Terrain>& terrain)
	{
		if (!modelComponent.meshes.empty())
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
