#pragma once
#include "IComponent.h"
#include "Engine/Scene/SceneCamera.h"

namespace Engine
{
	struct CameraComponent : public IComponent
	{
		bool primary = false;
		bool fixedAspectRatio = false;
		SceneCamera camera;
	};
}
