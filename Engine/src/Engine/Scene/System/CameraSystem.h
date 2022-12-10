#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	class CameraSystem
	{
	public:
		static void OnUpdate(float deltaTime, Transform *transform, SceneCamera *camera);
	};
}
