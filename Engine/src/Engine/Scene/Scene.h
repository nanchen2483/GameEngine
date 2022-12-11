#pragma once
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Physics/Collision/Collision.h"
#include "System/CameraSystem.h"
#include "Component/IComponent.h"

#include <entt/entt.hpp>

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Scene(bool enableShadow = true);
		~Scene();

		Entity CreateEntity(const std::string& name = "");
		void DestoryEntity(Entity entity);

		void OnUpdateEditor(EditorCamera& camera);
		void OnUpdateRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		bool EntityExists(entt::entity entity);
		Entity GetPrimaryCameraEntity();
	private:
		template<class T, typename std::enable_if<std::is_base_of<IComponent, T>::value>::type* = nullptr>
		void OnComponentAdded(Entity entity, T& component);
		
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
		Uniq<ShadowBox> m_shadowBox = nullptr;
		Uniq<Collision> m_collision = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class HierarchyPanel;
	};
}

