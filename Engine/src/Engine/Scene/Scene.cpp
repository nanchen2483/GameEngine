#include "enginepch.h"
#include "Scene.h"
#include "SceneSerializer.h"

#include "Component/CameraComponent.h"
#include "Component/MeshComponent.h"
#include "Component/TagComponent.h"

#include "Entity/Entity.h"
#include "Engine/Configuration/Configuration.h"
#include "Engine/Core/Window/Input.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"

#include "System/AnimationSystem.h"
#include "System/CameraSystem.h"
#include "System/PhysicsSystem.h"
#include "System/RendererSystem.h"
#include "System/MeshSystem.h"
#include "System/ScriptSystem.h"
#include "System/ShadowSystem.h"

namespace Engine
{
	Scene::Scene(const Ptr<IFramebuffer>& framebuffer)
		: m_framebuffer(framebuffer)
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

	void Scene::OnUpdateEditor(const EditorCamera& camera)
	{
		if (m_registry.empty())
		{
			return;
		}

		Renderer3D::ResetStates();
		Frustum frustum = camera.GetFrustum();

		// Update
		Entity terrainEntity = GetTerrainEntity();
		static Ptr<ITerrain> terrain;
		if (terrainEntity)
		{
			terrain = terrainEntity.GetComponent<TerrainComponent>().terrain;
			if (terrain != nullptr)
			{
				terrain->OnUpdate(camera.GetPosition());
			}
		}

		ScriptSystem::OnUpdate(m_registry, this);
		AnimationSystem::OnUpdate(m_registry);
		ShadowSystem::OnUpdate(m_registry,
			camera.GetViewMatrix(),
			camera.GetFOV(),
			camera.GetAspectRatio());

		// Debug
		m_framebuffer->Bind();
		Debug();

		// Draw
		RendererSystem::OnUpdate(m_registry,
			camera.GetViewMatrix(),
			camera.GetProjection(),
			camera.GetPosition(),
			frustum);
	}

	void Scene::Debug()
	{
		if (Configuration::ShowBoundingBox())
		{
			PhysicsSystem::DrawBoudingBox(m_registry);
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
			CameraComponent& primaryCamera = cameraEntity.GetComponent<CameraComponent>();
			TransformComponent& playerTransform = playerEntity.GetComponent<TransformComponent>();
			Frustum frustum = CameraSystem::GetFrustum(playerTransform.transform, primaryCamera.camera);
			Frustum lightFrustum = frustum.GetLightViewFrustum(LIGHT_DIRECTION);

			if (!Input::IsCursorVisible())
			{
				CameraSystem::OnUpdate(playerTransform.transform, primaryCamera.camera);
			}

			// Update
			Entity terrainEntity = GetTerrainEntity();
			static Ptr<ITerrain> terrain;
			if (terrainEntity)
			{
				terrain = terrainEntity.GetComponent<TerrainComponent>().terrain;
				if (terrain != nullptr)
				{
					terrain->OnUpdate(playerTransform.GetTranslation());
				}
			}

			const glm::mat4& viewMatrix = CameraSystem::GetViewMatrix(playerTransform);
			ScriptSystem::OnUpdate(m_registry, this);
			MeshSystem::OnUpdate(m_registry, frustum, lightFrustum, terrain);
			PhysicsSystem::OnUpdate(m_registry);
			AnimationSystem::OnUpdate(m_registry);
			ShadowSystem::OnUpdate(m_registry,
				viewMatrix,
				primaryCamera.camera.GetFOV(),
				primaryCamera.camera.GetAspectRatio());

			// Draw
			m_framebuffer->Bind();
			RendererSystem::OnUpdate(m_registry,
				viewMatrix,
				primaryCamera.camera.GetProjection(),
				playerTransform.GetTranslation(),
				frustum);
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

	void Scene::Serialize(const std::string& filepath)
	{
		SceneSerializer::Serialize(this, filepath);
	}

	void Scene::SerializeRuntime(const std::string& filepath)
	{
		SceneSerializer::SerializeRuntime(this, filepath);
	}

	void Scene::Deserialize(const std::string& filepath)
	{
		SceneSerializer::Deserialize(this, filepath);
	}

	void Scene::DeserializeRuntime(const std::string& filepath)
	{
		SceneSerializer::DeserializeRuntime(this, filepath);
	}
}
