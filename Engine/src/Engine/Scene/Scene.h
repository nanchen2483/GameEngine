#pragma once
#include "Engine/Core/TimeStep.h"
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Physics/Collision/Collision.h"

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

		void OnUpdateEditor(TimeStep time, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep time);
		void OnViewportResize(uint32_t width, uint32_t height);

		bool EntityExists(entt::entity entity);
		Entity GetPrimaryCameraEntity();
		Ptr<TextureMap> GetLoadedTextureMap() { return m_textureMap; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;
		Ptr<TextureMap> m_textureMap = nullptr;
		Uniq<ShadowBox> m_shadowBox = nullptr;
		Uniq<Collision> m_collision = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}

