#pragma once
#include "Component/IComponent.h"
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Buffer/Framebuffer.h"

#include <entt/entt.hpp>

namespace Engine
{
	class Entity;
	class SceneSerializer;

	class Scene
	{
	public:
		Scene(const Ptr<Framebuffer>& framebuffer);

		Entity CreateEntity(const std::string& name = "");
		void DestoryEntity(Entity entity);

		void OnUpdateEditor(const EditorCamera& camera);
		void OnUpdateRuntime();
		void OnViewportResize(uint32_t width, uint32_t height);

		bool EntityExists(entt::entity entity);
		Entity GetPrimaryCameraEntity();

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);
		void Deserialize(const std::string& filepath);
		void DeserializeRuntime(const std::string& filepath);
	private:
		Entity GetPlayerEntity();
		Entity GetTerrainEntity();
		void Debug();
		
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
		Ptr<Framebuffer> m_framebuffer;
		Uniq<SceneSerializer> m_serializer;

		friend class Entity;
		friend class SceneSerializer;
		friend class HierarchyPanel;
	};
}

