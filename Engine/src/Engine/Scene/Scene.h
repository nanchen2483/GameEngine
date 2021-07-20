#pragma once

#include <entt.hpp>
#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/EditorCamera.h"

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");
		void DestoryEntity(Entity entity);

		void OnUpdateEditor(TimeStep time, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep time);
		void OnViewportResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}

