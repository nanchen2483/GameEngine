#pragma once
#include "ISystem.h"
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Camera/Frustum.h"

#include <entt/entt.hpp>

namespace Engine
{
	class RendererSystem : public ISystem
	{
	public:
		static void OnUpdate(entt::registry& registry, const glm::mat4& cameraViewMatrix, const glm::mat4& cameraProjection, const glm::vec3& cameraPosition, const Frustum& frustum);
	};
}
