#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Terrain/Terrain.h"

namespace Engine
{
	struct TerrainComponent : public IComponent
	{
		Ptr<Terrain> terrain = nullptr;
		Ptr<Texture2D> texture = nullptr;
	};
}