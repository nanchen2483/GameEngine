#pragma once
#include "ISystem.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Terrain/Terrain.h"

#include <entt/entt.hpp>

namespace Engine
{
	class MeshSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry, const Frustum& frustum, const Frustum& lightViewFrustum, const Ptr<Terrain>& terrain);
	};
}
