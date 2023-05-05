#include "enginepch.h"
#include "ShadowSystem.h"
#include "Engine/Renderer/Renderer3D.h"
#include "Engine/Scene/Component/AnimationComponent.h"
#include "Engine/Scene/Component/MeshComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"

namespace Engine
{
	ShadowSystem::ShadowSystem()
	{
		m_shadowBox = CreateUniq<ShadowBox>();
	}

	void ShadowSystem::OnUpdate(entt::registry& registry, const glm::mat4& viewMatrix, float FOV, float aspectRatio)
	{
		ShadowSystem& instance = GetInstance();

		const Uniq<ShadowBox>& shadowBox = instance.m_shadowBox;
		shadowBox->Update(viewMatrix, FOV, aspectRatio);
		shadowBox->Bind();
		registry.view<TransformComponent, MeshComponent>()
			.each([&](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
				{
					Ptr<Animation> animation = nullptr;
					if (registry.all_of<AnimationComponent>(entity))
					{
						animation = registry.get<AnimationComponent>(entity);
					}

					Renderer3D::Draw(transform, mesh, animation, shadowBox->GetShader());
				});
		shadowBox->Ubind();
		shadowBox->BindTexture();
	}

	ShadowSystem& ShadowSystem::GetInstance()
	{
		static ShadowSystem instance;
		return instance;
	}
}
