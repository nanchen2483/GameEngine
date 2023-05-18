#pragma once
#include "Engine/Renderer/Terrain/ITerrain.h"
#include "Engine/Physics/BoundingBox/IBoundingBox.h"
#include "TerrainNode.h"

namespace Engine
{
	class QuadtreeTerrain : public ITerrain
	{
	public:
		QuadtreeTerrain(std::string filePath, int32_t entityId);
		QuadtreeTerrain(Ptr<ITexture2D> heightMapTexture, int32_t entityId);

		inline virtual std::string GetFilePath() const override { return m_buffer->GetHeightMapPath(); }
		virtual float GetHeight(float x, float z) const override { return m_buffer->GetTerrainHeight(x, z); }
		virtual TerrainType GetType() const override { return TerrainType::Quadtree; };

		virtual void OnUpdate(glm::vec3 cameraPosition) override;
		virtual void Draw(glm::mat4 model, const Frustum& frustum) override;
	private:
		Ptr<TerrainBuffer> m_buffer = nullptr;
		std::vector<TerrainNode> m_children;
		std::vector<Ptr<IBoundingBox>> m_boundingBoxes;
	};
}
