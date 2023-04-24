#pragma once
#include "ISystem.h"
#include "Engine/Scene/Scene.h"

#include <entt/entt.hpp>

namespace Engine
{
	class ScriptSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry, Scene* scene);
	};
}
