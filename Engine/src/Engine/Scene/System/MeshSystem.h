#pragma once
#include "ISystem.h"
#include "Engine/Renderer/Terrain/Terrain.h"
#include "Engine/Scene/Component/MeshComponent.h"
#include "Engine/Scene/Component/PhysicsComponent.h"

namespace Engine
{
	class MeshSystem : public ISystem
	{
	public:
		static void OnUpdate(MeshComponent& meshComponent, Transform& transform, PhysicsComponent physics, const Frustum& frustum, const Ptr<Terrain>& terrain);
	};
}
