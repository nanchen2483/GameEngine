#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Terrain/ITerrain.h"
#include "Engine/Renderer/Texture/ITexture.h"

namespace Engine
{
	struct ENGINE_API TerrainComponent : public IComponent
	{
		Ptr<ITerrain> terrain{};
		Ptr<ITexture2D> texture{};
	};
}
