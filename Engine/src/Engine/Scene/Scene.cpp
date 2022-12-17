#include "enginepch.h"
#include "Scene.h"

#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/ModelComponent.h"
#include "Component/NativeScriptComponent.h"
#include "Component/SkyboxComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TagComponent.h"
#include "Component/TerrainComponent.h"

#include "Entity.h"
#include "Engine/Core/Window/Input.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"

#include "System/CameraSystem.h"
#include "System/CollisionSystem.h"
#include "System/ModelSystem.h"
#include "System/ShadowSystem.h"

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
		TagComponent& tagComp = entity.AddComponent<TagComponent>();
		tagComp.tag = name.empty() ? "Entity" : name;
		return entity;
	}

	void Scene::DestoryEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(EditorCamera& camera)
	{
		if (m_registry.empty())
		{
			return;
		}

		Frustum frustum = camera.GetFrustum();
			
		// Update
		Entity terrainEntity = GetTerrainEntity();
		static Ptr<Terrain> terrain;
		if (terrainEntity)
		{
			terrain = terrainEntity.GetComponent<TerrainComponent>().terrain;
			if (terrain != nullptr)
			{
				terrain->OnUpdate(camera.GetPosition());
			}
		}

		auto modelView = m_registry.view<TransformComponent, ModelComponent>();
		modelView.each([&](TransformComponent& thisTransform, ModelComponent& thisComponent)
			{
				modelView.each([&](TransformComponent& thatTransform, ModelComponent& thatComponent)
					{
						CollisionSystem::OnUpdate(thisTransform, thatTransform, thisComponent, thatComponent);
					});
					
				ModelSystem::OnUpdate(thisComponent, thisTransform, frustum, terrain);
			});

		// Draw
		uint32_t numOfLights = m_registry.view<LightComponent>().size();
		Renderer3D::BeginScene(camera.GetViewMatrix(), camera.GetProjection(), camera.GetPosition(), numOfLights);

		m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>)
			.each([](entt::entity entity, TransformComponent& transform, SpriteRendererComponent& component)
				{
					Renderer3D::Draw(transform, component, (int)entity);
				});

		m_registry.view<TransformComponent, LightComponent>()
			.each([](entt::entity entity, TransformComponent& transform, LightComponent& component)
				{
					Renderer3D::Draw(transform, component, (int)entity);
				});

		Renderer3D::EndScene();

		m_registry.view<TransformComponent, ModelComponent>()
			.each([&](TransformComponent& thisTransform, ModelComponent& thisComponent)
				{
					Renderer3D::Draw(thisTransform, thisComponent);
				});

		m_registry.view<TransformComponent, TerrainComponent>()
			.each([&](TransformComponent& transform, TerrainComponent& component)
				{
					Renderer3D::Draw(transform, component, frustum);
				});

		m_registry.view<SkyboxComponent>()
			.each([](SkyboxComponent& component)
				{
					Renderer3D::Draw(component);
				});

		ShadowSystem::OnUpdate(camera.GetViewMatrix(), camera.GetFOV(), camera.GetAspectRatio(),
			[=]()
			{
				m_registry.view<TransformComponent, ModelComponent>()
					.each([=](TransformComponent& transformComponent, ModelComponent& modelComponent)
						{
							Renderer3D::Draw(transformComponent, modelComponent, ShadowSystem::GetShader());
						});
			});
	}

	void Scene::OnUpdateRuntime()
	{
		if (m_registry.empty())
		{
			return;
		}

		Entity cameraEntity = GetPrimaryCameraEntity();
		Entity playerEntity = GetPlayerEntity();
		if (cameraEntity && playerEntity)
		{
			CameraComponent &primaryCamera = cameraEntity.GetComponent<CameraComponent>();
			TransformComponent &playerTransform = playerEntity.GetComponent<TransformComponent>();
			Frustum frustum = primaryCamera.camera.GetFrustum(playerTransform);

			if (!Input::IsCursorVisible())
			{
				CameraSystem::OnUpdate(&playerTransform.transform, &primaryCamera.camera);
			}

			// Script
			{
				m_registry.view<NativeScriptComponent>()
					.each([=](entt::entity entity, NativeScriptComponent& nsc)
						{
							if (!nsc.instance)
							{
								nsc.instance = nsc.InstantiateScript();
								nsc.instance->m_entity = Entity{ entity, this };
								nsc.instance->OnCreate();
							}

							nsc.instance->OnUpdate();
						});
			}

			// Update
			Entity terrainEntity = GetTerrainEntity();
			static Ptr<Terrain> terrain;
			if (terrainEntity)
			{
				terrain = terrainEntity.GetComponent<TerrainComponent>().terrain;
				if (terrain != nullptr)
				{
					terrain->OnUpdate(playerTransform.GetTranslation());
				}
			}

			auto modelView = m_registry.view<TransformComponent, ModelComponent>();
			modelView.each([&](TransformComponent& thisTransform, ModelComponent& thisComponent)
				{
					modelView.each([&](TransformComponent& thatTransform, ModelComponent& thatComponent)
						{
							CollisionSystem::OnUpdate(thisTransform, thatTransform, thisComponent, thatComponent);
						});
					
					ModelSystem::OnUpdate(thisComponent, thisTransform, frustum, terrain);
				});

			// Draw
			glm::mat4 viewMatrix = CameraSystem::GetViewMatrix(playerTransform);
			uint32_t numOflights = m_registry.view<LightComponent>().size();
			Renderer3D::BeginScene(viewMatrix, primaryCamera.camera.GetProjection(), playerTransform.GetTranslation(), numOflights);

			m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>)
				.each([](TransformComponent& transform, SpriteRendererComponent& component)
					{
						Renderer3D::Draw(transform, component);
					});

			m_registry.view<TransformComponent, LightComponent>()
				.each([](TransformComponent& transform, LightComponent& component)
					{
						Renderer3D::Draw(transform, component);
					});

			Renderer3D::EndScene();

			m_registry.view<TransformComponent, ModelComponent>()
				.each([&](TransformComponent& thisTransform, ModelComponent& thisComponent)
					{
						Renderer3D::Draw(thisTransform, thisComponent);
					});

			m_registry.view<TransformComponent, TerrainComponent>()
				.each([&](TransformComponent& transform, TerrainComponent& component)
					{
						Renderer3D::Draw(transform, component, frustum);
					});

			m_registry.view<SkyboxComponent>()
				.each([](SkyboxComponent& component)
					{
						Renderer3D::Draw(component);
					});

			ShadowSystem::OnUpdate(viewMatrix, primaryCamera.camera.GetFOV(), primaryCamera.camera.GetAspectRatio(),
				[=]()
				{
					m_registry.view<TransformComponent, ModelComponent>()
						.each([=](TransformComponent& transformComponent, ModelComponent& modelComponent)
							{
								Renderer3D::Draw(transformComponent, modelComponent, ShadowSystem::GetShader());
							});
				});
		}
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;

		auto view = m_registry.view<CameraComponent>();
		for (entt::entity entity : view)
		{
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
			{
				cameraComponent.camera.SetViewportSize(width, height);
			}
		}
	}

	bool Scene::EntityExists(entt::entity entity)
	{
		return m_registry.valid(entity);
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (entt::entity entity : view)
		{
			const CameraComponent& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
			{
				return Entity(entity, this);
			}
		}
		return {};
	}

	Entity Scene::GetPlayerEntity()
	{
		auto view = m_registry.view<ModelComponent>();
		for (entt::entity entity : view)
		{
			const ModelComponent& camera = view.get<ModelComponent>(entity);
			if (camera.isPlayer)
			{
				return Entity(entity, this);
			}
		}
		return {};
	}

	Entity Scene::GetTerrainEntity()
	{
		auto view = m_registry.view<TerrainComponent>();
		for (entt::entity entity : view)
		{
			return Entity(entity, this);
		}
		return {};
	}

	template<typename T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type*>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<LightComponent>(Entity entity, LightComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SkyboxComponent>(Entity entity, SkyboxComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TerrainComponent>(Entity entity, TerrainComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}
