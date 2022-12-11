#pragma once
#include "ISystem.h"
#include "Engine/Math/Transform.h"
#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	class CameraSystem : public ISystem
	{
	public:
		static void Update(Transform *transform, SceneCamera *camera);
		static glm::mat4 CalculateViewMatrix(const Transform& transform) { return glm::inverse((glm::mat4)transform); }
	};
}
