#include "enginepch.h"
#include "Scene.h"

#include "Component.h"
#include "Entity.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}
	
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::OnUpdate(TimeStep time)
	{
		Camera* mainCamera = nullptr;
		glm::mat4* mainTrnasform = nullptr;
		{
			auto group = m_registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.primary)
				{
					mainCamera = &camera.camera;
					mainTrnasform = &transform.transform;
					break;
				}
			}
		}

		if (mainCamera != nullptr)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *mainTrnasform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawQuad(transform, sprite.color);
			}

			Renderer2D::EndScene();
		}

	}
}