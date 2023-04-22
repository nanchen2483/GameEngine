#include "enginepch.h"
#include "Scene.h"

#include "Component/AnimationComponent.h"
#include "Component/CameraComponent.h"
#include "Component/PhysicsComponent.h"
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
#include "System/PhysicsSystem.h"
#include "System/MeshSystem.h"
#include "System/ShadowSystem.h"

namespace Engine
{
	Scene::Scene(const Ptr<Framebuffer>& framebuffer)
		: m_framebuffer(framebuffer)
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

		m_registry.view<AnimationComponent>()
			.each([=](AnimationComponent& component)
				{
					if (component.isEnabled)
					{
						AnimationSystem::UpdateAnimation(component);
					}
				});

		ShadowSystem::OnUpdate(camera.GetViewMatrix(), camera.GetFOV(), camera.GetAspectRatio(),
			[=]()
			{
				m_registry.view<TransformComponent, MeshComponent>()
					.each([=](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
						{
							Ptr<Animation> animation = nullptr;
							if (m_registry.all_of<AnimationComponent>(entity))
							{
								animation = m_registry.get<AnimationComponent>(entity);
							}

							Renderer3D::Draw(transform, mesh, animation, ShadowSystem::GetShader());
						});
			});

		// Debug
		m_framebuffer->Bind();
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
	}

	void Scene::Debug()
	{
		if (Configuration::GetInstance()->ShowBoundingBox())
		{
			m_registry.view<TransformComponent, PhysicsComponent>()
				.each([=](TransformComponent& transform, PhysicsComponent& component)
					{
						PhysicsSystem::DrawBoudingBox(transform, component.boundingBox);
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
			Frustum frustum = CameraSystem::GetFrustum(playerTransform.transform, primaryCamera.camera);
			Frustum lightFrustum = frustum.GetLightViewFrustum(LIGHT_DIRECTION);

			if (!Input::IsCursorVisible())
			{
				CameraSystem::OnUpdate(playerTransform.transform, primaryCamera.camera);
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

			auto meshView = m_registry.view<TransformComponent, MeshComponent, PhysicsComponent>();
			meshView.each([&](TransformComponent& thisTransform, MeshComponent& meshComponent, PhysicsComponent& thisComponent)
				{
					MeshSystem::OnUpdate(meshComponent, thisTransform, thisComponent, frustum, lightFrustum, terrain);
					meshView.each([&](TransformComponent& thatTransform, MeshComponent& meshComponent, PhysicsComponent& thatComponent)
						{
							PhysicsSystem::OnUpdate(thisTransform, thatTransform, &thisComponent, &thatComponent);
						});
				});

			m_registry.view<AnimationComponent>()
				.each([](AnimationComponent& component)
					{
						if (component.isEnabled)
						{
							AnimationSystem::UpdateAnimation(component);
						}
					});

			glm::mat4 viewMatrix = CameraSystem::GetViewMatrix(playerTransform);
			ShadowSystem::OnUpdate(viewMatrix, primaryCamera.camera.GetFOV(), primaryCamera.camera.GetAspectRatio(),
				[=]()
				{
					m_registry.view<TransformComponent, MeshComponent>()
						.each([&](entt::entity entity, TransformComponent& transform, MeshComponent& mesh)
							{
								if (mesh.isOnLightViewFrustum)
								{
									Ptr<Animation> animation = nullptr;
									if (m_registry.all_of<AnimationComponent>(entity))
									{
										animation = m_registry.get<AnimationComponent>(entity);
									}

									Renderer3D::Draw(transform, mesh, animation, ShadowSystem::GetShader());
								}
							});
				});

			// Draw
			m_framebuffer->Bind();
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
						if (mesh.isOnViewFrustum)
						{
							Ptr<Animation> animation = nullptr;
							if (m_registry.all_of<AnimationComponent>(entity))
							{
								animation = m_registry.get<AnimationComponent>(entity);
							}

							Renderer3D::Draw(transform, mesh, animation);
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
}
