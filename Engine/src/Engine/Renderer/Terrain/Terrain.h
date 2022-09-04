#pragma once
#include "Engine/Renderer/Texture/Texture.h"
#include <glm/glm.hpp>
#include <Engine/Renderer/Camera/Frustum.h>

namespace Engine
{
	enum class TerrainType
	{
		Default = 1,
		Quadtree = 2,
	};

	class Terrain
	{
	public:
		virtual std::string GetFilePath() const = 0;
		virtual float GetHeight(float x, float z) const  = 0;
		virtual TerrainType GetType() const = 0;
		virtual void OnUpdate(glm::vec3 position) = 0;
		virtual void Draw(glm::mat4 model, const Frustum& frustum) = 0;
	
		static Uniq<Terrain> Create(TerrainType type, std::string filePath, int32_t entityId = -1);
		static Uniq<Terrain> Create(TerrainType type, Ptr<Texture2D> texture, int32_t entityId = -1);
	};
}
