#pragma once
#include "ISystem.h"
#include "Engine/Math/Transform.h"
#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	class CameraSystem : public ISystem
	{
	public:
		static void OnUpdate(Transform& transform, SceneCamera& camera);
		static glm::mat4 GetViewMatrix(const Transform& transform) { return glm::inverse((glm::mat4)transform); }
		static Frustum GetFrustum(const Transform& transform, const SceneCamera& camera);
	};
}
