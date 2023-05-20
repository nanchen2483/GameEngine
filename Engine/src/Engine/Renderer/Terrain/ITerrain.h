#pragma once
#include "Engine/Renderer/Camera/Frustum.h"
#include <glm/glm.hpp>

namespace Engine
{
	enum class TerrainType
	{
		Default = 1,
		Quadtree = 2,
	};

	class ITerrain
	{
	public:
		virtual std::string GetFilePath() const = 0;
		virtual float GetHeight(float x, float z) const  = 0;
		virtual TerrainType GetType() const = 0;
		virtual void OnUpdate(glm::vec3 cameraPosition) = 0;
		virtual void Draw(glm::mat4 model, const Frustum& frustum) = 0;
	};
}
