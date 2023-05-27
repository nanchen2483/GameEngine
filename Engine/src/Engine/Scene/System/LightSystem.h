#pragma once
#include "ISystem.h"

#include <entt/entt.hpp>
#include <vector>

namespace Engine
{
	class LightSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry);
		static std::vector<std::vector<const void*>> GetLights() { return GetInstance().m_lights; }
	private:
		LightSystem() = default;
		static LightSystem& GetInstance();

		std::vector<std::vector<const void*>> m_lights;
	};
}
