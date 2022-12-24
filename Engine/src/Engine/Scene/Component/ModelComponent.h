#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Model.h"

namespace Engine
{
	struct ModelComponent : public IComponent
	{
		bool isPlayer = false;
		bool loading = false;
		Ptr<float> progression = CreatePtr<float>(0.0f);
		bool isOnViewFrustum = false;

		bool enableAnimation = false;
		Ptr<Model> model = nullptr;
	};
}