#include "enginepch.h"
#include "Scene.h"

#include "Component/AnimationComponent.h"
#include "Component/CameraComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/LightComponent.h"
#include "Component/NativeScriptComponent.h"
#include "Component/SkyboxComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TagComponent.h"
#include "Component/TerrainComponent.h"

#include "Entity.h"
#include "Engine/Configuration/Configuration.h"
#include "Engine/Core/Window/Input.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"

#include "System/AnimationSystem.h"
#include "System/CameraSystem.h"
#include "System/CollisionSystem.h"
#include "System/MeshSystem.h"
#include "System/ShadowSystem.h"

namespace Engine
{
	Scene::Scene()
	{
		m_boudingBoxDebug = CreatePtr<BoudingBoxDebug>();
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

		Renderer3D::ResetStates();
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

		auto meshView = m_registry.view<TransformComponent, MeshComponent, CollisionComponent>();
		meshView.each([&](TransformComponent& thisTransform, MeshComponent& meshComponent, CollisionComponent& thisComponent)
			{
				meshView.each([&](TransformComponent& thatTransform, MeshComponent& meshComponent, CollisionComponent& thatComponent)
					{
						CollisionSystem::OnUpdate(thisTransform, thatTransform, thisComponent, thatComponent);
					});
					
				MeshSystem::OnUpdate(meshComponent, thisComponent, thisTransform, frustum, terrain);
			});

		m_registry.view<AnimationComponent>()
			.each([=](AnimationComponent& component)
				{
					if (component.isEnabled)
					{
						AnimationSystem::UpdateAnimation(component);
					}
				});

		// Debug
		Debug();

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

		m_registry.view<TransformComponent, MeshComponent>()
			.each([&](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
				{
					Ptr<Animation> animation = nullptr;
					if (m_registry.all_of<AnimationComponent>(entity))
					{
						animation = m_registry.get<AnimationComponent>(entity);
					}

					Renderer3D::Draw(transform, mesh, animation, nullptr, (int)entity);
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
				m_registry.view<TransformComponent, MeshComponent>()
					.each([=](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
						{
							if (m_registry.all_of<AnimationComponent>(entity))
							{
								Renderer3D::Draw(transform, mesh, m_registry.get<AnimationComponent>(entity), ShadowSystem::GetShader());
							}
							else
							{
								Renderer3D::Draw(transform, mesh, nullptr, ShadowSystem::GetShader());
							}
						});
			});
	}

	void Scene::Debug()
	{
		if (Configuration::GetInstance()->ShowBoudingBox())
		{
			m_registry.view<TransformComponent, CollisionComponent>()
				.each([=](TransformComponent& transform, CollisionComponent& component)
					{
						m_boudingBoxDebug->Draw(transform, component.boundingBox->GetBoundingValue());
					});
		}
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

			auto meshView = m_registry.view<TransformComponent, MeshComponent, CollisionComponent>();
			meshView.each([&](TransformComponent& thisTransform, MeshComponent& meshComponent, CollisionComponent& thisComponent)
				{
					meshView.each([&](TransformComponent& thatTransform, MeshComponent& meshComponent, CollisionComponent& thatComponent)
						{
							CollisionSystem::OnUpdate(thisTransform, thatTransform, thisComponent, thatComponent);
						});
					
					MeshSystem::OnUpdate(meshComponent, thisComponent, thisTransform, frustum, terrain);
				});

			m_registry.view<AnimationComponent>()
				.each([](AnimationComponent& component)
					{
						AnimationSystem::UpdateAnimation(component);
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

			m_registry.view<TransformComponent, MeshComponent>()
				.each([&](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
					{
						if (m_registry.all_of<AnimationComponent>(entity))
						{
							Renderer3D::Draw(transform, mesh, m_registry.get<AnimationComponent>(entity));
						}
						else
						{
							Renderer3D::Draw(transform, mesh);
						}
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
					m_registry.view<TransformComponent, MeshComponent>()
						.each([=](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
							{
								if (m_registry.all_of<AnimationComponent>(entity))
								{
									Renderer3D::Draw(transform, mesh, m_registry.get<AnimationComponent>(entity), ShadowSystem::GetShader());
								}
								else
								{
									Renderer3D::Draw(transform, mesh, nullptr, ShadowSystem::GetShader());
								}
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
		auto view = m_registry.view<MeshComponent>();
		for (entt::entity entity : view)
		{
			const MeshComponent& mesh = view.get<MeshComponent>(entity);
			if (mesh.isPlayer)
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
	void Scene::OnComponentAdded<MeshComponent>(Entity entity, MeshComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<AnimationComponent>(Entity entity, AnimationComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CollisionComponent>(Entity entity, CollisionComponent& component)
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
