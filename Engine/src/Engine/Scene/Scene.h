#pragma once
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Component/IComponent.h"

#include <entt/entt.hpp>

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Scene(const Ptr<Framebuffer>& framebuffer);
		~Scene();

		Entity CreateEntity(const std::string& name = "");
		void DestoryEntity(Entity entity);

		void OnUpdateEditor(EditorCamera& camera);
		void OnUpdateRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		bool EntityExists(entt::entity entity);
		Entity GetPrimaryCameraEntity();
		Entity GetPlayerEntity();
		Entity GetTerrainEntity();
	private:
		void Debug();
		
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
		Ptr<Framebuffer> m_framebuffer;

		friend class Entity;
		friend class SceneSerializer;
		friend class HierarchyPanel;
	};
}

