#pragma once
#include "ITerrain.h"
#include "Engine/Renderer/Texture/ITexture.h"

#include <glm/glm.hpp>

namespace Engine
{
	class ENGINE_API TerrainFactory
	{
	public:
		static Uniq<ITerrain> Create(TerrainType type, std::string filePath, int32_t entityId = -1);
		static Uniq<ITerrain> Create(TerrainType type, Ptr<ITexture2D> texture, int32_t entityId = -1);
	};
}
