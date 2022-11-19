#pragma once
#include "Engine/Renderer/Terrain/Terrain.h"
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "TerrainNode.h"

namespace Engine
{
	class QuadtreeTerrain : public Terrain
	{
	public:
		QuadtreeTerrain(std::string filePath, int32_t entityId);
		QuadtreeTerrain(Ptr<Texture2D> heightMapTexture, int32_t entityId);

		inline virtual std::string GetFilePath() const override { return m_buffer->GetHeightMapPath(); }
		virtual float GetHeight(float x, float z) const override { return m_buffer->GetTerrainHeight(x, z); }
		virtual TerrainType GetType() const override { return TerrainType::Quadtree; };

		virtual void OnUpdate(glm::vec3 position) override;
		virtual void Draw(glm::mat4 model, const Frustum& frustum) override;
	private:
		Ptr<TerrainBuffer> m_buffer = nullptr;
		std::vector<TerrainNode> m_children;
		std::vector<Uniq<BoundingBox>> m_boundingBoxes;
	};
}
