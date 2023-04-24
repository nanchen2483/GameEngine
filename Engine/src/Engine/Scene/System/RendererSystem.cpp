#include "enginepch.h"
#include "RendererSystem.h"

#include "Engine/Scene/Component/AnimationComponent.h"
#include "Engine/Scene/Component/LightComponent.h"
#include "Engine/Scene/Component/MeshComponent.h"
#include "Engine/Scene/Component/SkyboxComponent.h"
#include "Engine/Scene/Component/SpriteRendererComponent.h"
#include "Engine/Scene/Component/TerrainComponent.h"

#include "Engine/Renderer/Renderer3D.h"

namespace Engine
{
	void RendererSystem::OnUpdate(entt::registry& registry, const glm::mat4& cameraViewMatrix, const glm::mat4& cameraProjection, const glm::vec3& cameraPosition, const Frustum& frustum)
	{
		uint32_t numOfLights = registry.view<LightComponent>().size();
		Renderer3D::BeginScene(cameraViewMatrix, cameraProjection, cameraPosition, numOfLights);

		registry.group<TransformComponent>(entt::get<SpriteRendererComponent>)
			.each([](entt::entity entity, TransformComponent& transform, SpriteRendererComponent& component)
				{
					Renderer3D::Draw(transform, component, (int)entity);
				});

		registry.view<TransformComponent, LightComponent>()
			.each([](entt::entity entity, TransformComponent& transform, LightComponent& component)
				{
					Renderer3D::Draw(transform, component, (int)entity);
				});

		Renderer3D::EndScene();

		registry.view<TransformComponent, MeshComponent>()
			.each([&](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
				{
					Ptr<Animation> animation = nullptr;
					if (registry.all_of<AnimationComponent>(entity))
					{
						animation = registry.get<AnimationComponent>(entity);
					}

					Renderer3D::Draw(transform, mesh, animation, nullptr, (int)entity);
				});

		registry.view<TransformComponent, TerrainComponent>()
			.each([&](TransformComponent& transform, TerrainComponent& component)
				{
					Renderer3D::Draw(transform, component, frustum);
				});

		registry.view<SkyboxComponent>()
			.each([](SkyboxComponent& component)
				{
					Renderer3D::Draw(component);
				});
	}
}
