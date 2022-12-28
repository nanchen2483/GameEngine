#include "enginepch.h"
#include "Terrain.h"
#include "Tessellation/TessellationTerrain.h"
#include "Quadtree/QuadtreeTerrain.h"

namespace Engine
{
	Uniq<Terrain> Terrain::Create(TerrainType type, std::string filePath, int32_t entityId)
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
	
	Uniq<Terrain> Terrain::Create(TerrainType type, Ptr<Texture2D> texture, int32_t entityId)
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
