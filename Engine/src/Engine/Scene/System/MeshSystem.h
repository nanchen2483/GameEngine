#pragma once
#include "ISystem.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Terrain/ITerrain.h"

#include <entt/entt.hpp>

namespace Engine
{
	class MeshSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry, const Frustum& frustum, const Frustum& lightViewFrustum, const Ptr<ITerrain>& terrain);
	};
}
