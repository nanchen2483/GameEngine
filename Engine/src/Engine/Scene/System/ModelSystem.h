#pragma once
#include "ISystem.h"
#include "Engine/Renderer/Terrain/Terrain.h"
#include "Engine/Scene/Component/ModelComponent.h"

namespace Engine
{
	class ModelSystem : public ISystem
	{
	public:
		static void OnUpdate(ModelComponent& modelComponent, Transform& modelTransform, const Frustum& frustum, const Ptr<Terrain>& terrain);
	};
}
