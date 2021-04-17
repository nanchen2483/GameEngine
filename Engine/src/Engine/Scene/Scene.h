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
	public:
		entt::registry m_registry;
	};
}

