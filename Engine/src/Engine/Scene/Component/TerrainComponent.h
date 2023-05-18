#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Terrain/ITerrain.h"

namespace Engine
{
	struct ENGINE_API TerrainComponent : public IComponent
	{
		Ptr<ITerrain> terrain = nullptr;
		Ptr<ITexture2D> texture = nullptr;
	};
}
