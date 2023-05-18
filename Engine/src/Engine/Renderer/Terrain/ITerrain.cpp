#include "enginepch.h"
#include "ITerrain.h"
#include "Tessellation/TessellationTerrain.h"
#include "Quadtree/QuadtreeTerrain.h"

namespace Engine
{
	Uniq<ITerrain> ITerrain::Create(TerrainType type, std::string filePath, int32_t entityId)
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
	
	Uniq<ITerrain> ITerrain::Create(TerrainType type, Ptr<ITexture2D> texture, int32_t entityId)
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
