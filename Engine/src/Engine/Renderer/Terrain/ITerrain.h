#pragma once
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Texture/ITexture.h"
#include <glm/glm.hpp>

namespace Engine
{
	enum class TerrainType
	{
		Default = 1,
		Quadtree = 2,
	};

	class ENGINE_API ITerrain
	{
	public:
		virtual std::string GetFilePath() const = 0;
		virtual float GetHeight(float x, float z) const  = 0;
		virtual TerrainType GetType() const = 0;
		virtual void OnUpdate(glm::vec3 cameraPosition) = 0;
		virtual void Draw(glm::mat4 model, const Frustum& frustum) = 0;
	
		static Uniq<ITerrain> Create(TerrainType type, std::string filePath, int32_t entityId = -1);
		static Uniq<ITerrain> Create(TerrainType type, Ptr<ITexture2D> texture, int32_t entityId = -1);
	};
}
