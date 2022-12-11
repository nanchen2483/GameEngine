#include "enginepch.h"
#include "Scene.h"

#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"
#include "Component/NativeScriptComponent.h"
#include "Component/ModelComponent.h"
#include "Component/SkyboxComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TagComponent.h"
#include "Component/TerrainComponent.h"

#include "Entity.h"
#include "Engine/Core/Window/Input.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"

namespace Engine
{
	Scene::Scene(bool enableShadow)
	{
		m_collision = Collision::Create(CollisionType::GJK_EPA_3D);
		if (enableShadow)
		{
			m_shadowBox = CreateUniq<ShadowBox>();
		}
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
		if (!m_registry.empty())
		{
			static TerrainComponent terrainComponent;
			Frustum frustum = camera.GetFrustum();
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

			auto modelView = m_registry.view<TransformComponent, ModelComponent>();
			modelView.each([&](entt::entity thisEntity, TransformComponent& thisTransform, ModelComponent& thisComponent)
					{
						bool isComparable = false;
						modelView.each([&](entt::entity thatEntity, TransformComponent& thatTransform, ModelComponent& thatComponent)
							{
								if (isComparable)
								{
									m_collision->Detect(thisTransform, thatTransform, thisComponent, thatComponent);
									if (m_collision->GetDistance() < 0)
									{
										glm::vec3 x = m_collision->GetDirectionFromAToB() / 2.0f;
										thisTransform.transform.translation += x;
										thatTransform.transform.translation -= x;
									}
								}

								if (thisEntity == thatEntity && thisComponent && thatComponent) { isComparable = true; }
							});
						
						terrainComponent.SetHeight(thisTransform);
						thisComponent.OnUpdate(frustum, thisTransform);
						Renderer3D::Draw(thisTransform, thisComponent);
					});

			m_registry.view<TransformComponent, TerrainComponent>()
				.each([&](TransformComponent& transform, TerrainComponent& component)
					{
						terrainComponent = component;
						component.OnUpdate(camera.GetPosition());
						Renderer3D::Draw(transform, component, frustum);
					});

			m_registry.view<SkyboxComponent>()
				.each([](SkyboxComponent& component)
					{
						Renderer3D::Draw(component);
					});

			if (m_shadowBox != nullptr)
			{
				m_shadowBox->Update(camera.GetViewMatrix(), camera.GetFOV(), camera.GetAspectRatio());
				m_shadowBox->Bind();
				Ptr<Shader> shader = m_shadowBox->GetShader();
				m_registry.view<TransformComponent, ModelComponent>()
					.each([=](TransformComponent& transform, ModelComponent& modelComponent)
						{
							if (modelComponent.isOnViewFrustum)
							{
								shader->SetMat4("uModel", transform);
								if (modelComponent.model->HasAnimations())
								{
									std::vector<glm::mat4> transforms = modelComponent.model->GetBoneTransforms();
									for (uint32_t i = 0; i < transforms.size(); i++)
									{
										shader->SetMat4("uBoneTransforms[" + std::to_string(i) + "]", transforms[i]);
									}
								}
								modelComponent.model->Draw();
							}
						});
				m_shadowBox->Ubind();
				m_shadowBox->BindTexture();
			}
		}
	}

	void Scene::OnUpdateRuntime()
	{
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

		CameraComponent* mainCamera = nullptr;
		{
			auto view = m_registry.view<CameraComponent>();
			for (entt::entity entity : view)
			{
				CameraComponent& camera = view.get<CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera;
					break;
				}
			}
		}

		TransformComponent* playerTransform = nullptr;
		auto view = m_registry.view<TransformComponent, ModelComponent>();
		for (entt::entity entity : view)
		{
			auto [transform, model] = view.get<TransformComponent, ModelComponent>(entity);
			if (model.isPlayer)
			{
				playerTransform = &transform;
				break;
			}
		}

		if (mainCamera != nullptr && playerTransform != nullptr)
		{
			if (!Input::IsCursorVisible())
			{
				CameraSystem::Update(&playerTransform->transform, &mainCamera->camera);
			}

			glm::mat4 viewMatrix = CameraSystem::CalculateViewMatrix(*playerTransform);
			Frustum frustum = mainCamera->camera.GetFrustum(*playerTransform);
			uint32_t numOflights = m_registry.view<LightComponent>().size();
			Renderer3D::BeginScene(viewMatrix, mainCamera->camera.GetProjection(), playerTransform->GetTranslation(), numOflights);

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
				.each([=](TransformComponent& transform, ModelComponent& component)
					{
						component.OnUpdate(frustum, transform);
						Renderer3D::Draw(transform, component);
					});

			m_registry.view<TransformComponent, TerrainComponent>()
				.each([&](TransformComponent& transform, TerrainComponent& component)
					{
						component.OnUpdate(playerTransform->GetTranslation());
						Renderer3D::Draw(transform, component, frustum);
					});

			m_registry.view<SkyboxComponent>()
				.each([](SkyboxComponent& component)
					{
						Renderer3D::Draw(component);
					});

			if (m_shadowBox != nullptr)
			{
				m_shadowBox->Update(viewMatrix, mainCamera->camera.GetFOV(), mainCamera->camera.GetAspectRatio());
				m_shadowBox->Bind();
				Ptr<Shader> shader = m_shadowBox->GetShader();
				m_registry.view<TransformComponent, ModelComponent>()
					.each([=](TransformComponent& transform, ModelComponent& modelComponent)
						{
							if (modelComponent.isOnViewFrustum)
							{
								shader->SetMat4("uModel", transform);
								if (modelComponent.model->HasAnimations())
								{
									std::vector<glm::mat4> transforms = modelComponent.model->GetBoneTransforms();
									for (uint32_t i = 0; i < transforms.size(); i++)
									{
										shader->SetMat4("uBoneTransforms[" + std::to_string(i) + "]", transforms[i]);
									}
								}
								modelComponent.model->Draw();
							}
						});
				m_shadowBox->Ubind();
				m_shadowBox->BindTexture();
			}
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
		return Entity();
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
