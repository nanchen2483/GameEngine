#pragma once

#include <entt.hpp>
#include <Engine/Core/TimeStep.h>

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");

		void OnUpdate(TimeStep time);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		entt::registry m_registry;
		uint32_t m_viewportWidth = 0, m_viewportHeight = 0;

		friend class Entity;
	};
}

