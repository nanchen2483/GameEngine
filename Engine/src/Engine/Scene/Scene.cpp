#include "enginepch.h"
#include "Scene.h"

#include "Component.h"
#include "Entity.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"

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

	void Scene::OnUpdateEditor(TimeStep time, EditorCamera& camera)
	{
		Renderer3D::BeginScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (entt::entity entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer3D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}

		Renderer3D::EndScene();

		auto modelGroup = m_registry.view<TransformComponent, ModelComponent>();
		for (entt::entity entity : modelGroup)
		{
			auto [transform, model] = modelGroup.get<TransformComponent, ModelComponent>(entity);
			Renderer3D::DrawModel(transform.GetTransform(), model);
		}

		auto animationGroup = m_registry.view<TransformComponent, SkeletonAnimationComponent>();
		for (entt::entity entity : animationGroup)
		{
			auto [transform, animation] = animationGroup.get<TransformComponent, SkeletonAnimationComponent>(entity);
			if (animation.model != nullptr)
			{
				animation.model->UpdateAnimation(time);
			}

			Renderer3D::DrawAnimation(transform.GetTransform(), animation);
		}
	}

	void Scene::OnUpdateRuntime(TimeStep time)
	{
		// Script
		{
			m_registry.view<NativeScriptComponent>().each([=](entt::entity entity, NativeScriptComponent& nsc) {
				if (!nsc.instance)
				{
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->m_entity = Entity{ entity, this };
					nsc.instance->OnCreate();
				}

				nsc.instance->OnUpdate(time);
			});
		}

		Camera* mainCamera = nullptr;
		glm::mat4 mainTransform;
		{
			auto view = m_registry.view<TransformComponent, CameraComponent>();
			for (entt::entity entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.primary)
				{
					mainCamera = &camera.camera;
					mainTransform = transform.GetTransform();
					break;
				}
			}
		}

		if (mainCamera != nullptr)
		{
			Renderer3D::BeginScene(*mainCamera, mainTransform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (entt::entity entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer3D::DrawSprite(transform.GetTransform(), sprite);
			}

			Renderer3D::EndScene();

			auto modelGroup = m_registry.view<TransformComponent, ModelComponent>();
			for (entt::entity entity : modelGroup)
			{
				auto [transform, model] = modelGroup.get<TransformComponent, ModelComponent>(entity);
				Renderer3D::DrawModel(transform.GetTransform(), model);
			}

			auto animationGroup = m_registry.view<TransformComponent, SkeletonAnimationComponent>();
			for (entt::entity entity : animationGroup)
			{
				auto [transform, animation] = animationGroup.get<TransformComponent, SkeletonAnimationComponent>(entity);
				if (animation.model != nullptr)
				{
					animation.model->UpdateAnimation(time);
				}

				Renderer3D::DrawAnimation(transform.GetTransform(), animation);
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

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component) {
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
	void Scene::OnComponentAdded<ModelComponent>(Entity entity, ModelComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SkeletonAnimationComponent>(Entity entity, SkeletonAnimationComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}
}
