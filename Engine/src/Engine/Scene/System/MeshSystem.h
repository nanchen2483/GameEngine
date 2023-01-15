#pragma once
#include "ISystem.h"
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "Engine/Renderer/Terrain/Terrain.h"
#include "Engine/Scene/Component/MeshComponent.h"

namespace Engine
{
	class MeshSystem : public ISystem
	{
	public:
		static void OnUpdate(MeshComponent& meshComponent, Ptr<BoundingBox> boundingBox, Transform& meshTransform, const Frustum& frustum, const Ptr<Terrain>& terrain);
	};
}
