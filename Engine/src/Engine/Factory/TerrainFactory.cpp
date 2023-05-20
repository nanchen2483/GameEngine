#include "enginepch.h"
#include "TerrainFactory.h"

#include "Engine/Renderer/Terrain/Quadtree/QuadtreeTerrain.h"
#include "Engine/Renderer/Terrain/Tessellation/TessellationTerrain.h"

namespace Engine
{
	Uniq<ITerrain> TerrainFactory::Create(TerrainType type, std::string filePath, int32_t entityId)
	{
		switch (type)
		{
		case TerrainType::Default:
			return CreateUniq<TessellationTerrain>(filePath, entityId);
		case TerrainType::Quadtree:
			return CreateUniq<QuadtreeTerrain>(filePath, entityId);
		default:
			ENGINE_CORE_ASSERT(false, "Unknown Terrain");
			return nullptr;
		}
	}

	Uniq<ITerrain> TerrainFactory::Create(TerrainType type, Ptr<ITexture2D> texture, int32_t entityId)
	{
		switch (type)
		{
		case TerrainType::Default:
			return CreateUniq<TessellationTerrain>(texture, entityId);
		case TerrainType::Quadtree:
			return CreateUniq<QuadtreeTerrain>(texture, entityId);
		default:
			ENGINE_CORE_ASSERT(false, "Unknown Terrain");
			return nullptr;
		}
	}
}
