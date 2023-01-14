#include "enginepch.h"
#include "ModelSystem.h"
#include "Engine/Renderer/Model/ModelLibrary.h"
#include "Engine/Scene/System/AnimationSystem.h"

namespace Engine
{
	void ModelSystem::Load(ModelComponent* modelComponent, const std::string& filepath, uint32_t entityId)
	{
		std::thread([=]()
			{
				modelComponent->loading = true;
				modelComponent->filePath = filepath;
				Ptr<Model> model = ModelLibrary::GetInstance()->Load(filepath, entityId);
				modelComponent->meshes = model->GetMeshes();
				modelComponent->animations = model->GetAnimations();
				modelComponent->boundingBox = model->GetBoundingBox();
				modelComponent->loading = false;
				*modelComponent->progression = 0.0f;
			}).detach();
	}

	void ModelSystem::Load(MeshComponent* meshComponent, const std::string& filepath, uint32_t entityId)
	{
		std::thread([=]()
			{
				meshComponent->isLoading = true;
				meshComponent->filePath = filepath;
				Ptr<Model> model = ModelLibrary::GetInstance()->Load(filepath, entityId);
				meshComponent->meshes = model->GetMeshes();
				meshComponent->isLoading = false;
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

			modelComponent.isOnViewFrustum = modelComponent.boundingBox->IsOnFrustum(frustum, modelTransform);
			if (modelComponent.enableAnimation && !modelComponent.animations.empty())
			{
				AnimationSystem::UpdateAnimation(modelComponent.animations[modelComponent.selectedAnimationIndex]);
			}
		}
	}
}
