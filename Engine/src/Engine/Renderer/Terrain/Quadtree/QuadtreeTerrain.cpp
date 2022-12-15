#include "enginepch.h"
#include "QuadtreeTerrain.h"

namespace Engine
{
	QuadtreeTerrain::QuadtreeTerrain(std::string filePath, int32_t entityId)
		: QuadtreeTerrain(Texture2D::Create(filePath, TextureType::Height, false), entityId)
	{
	}

	QuadtreeTerrain::QuadtreeTerrain(Ptr<Texture2D> heightMapTexture, int32_t entityId)
	{
		m_buffer = CreatePtr<TerrainBuffer>(heightMapTexture, entityId);
		for (uint32_t i = 0; i < TerrainBuffer::NUM_OF_ROOT_NODES; ++i)
		{
			for (uint32_t j = 0; j < TerrainBuffer::NUM_OF_ROOT_NODES; ++j)
			{
				glm::vec2 location = glm::vec2((float)i / (float)TerrainBuffer::NUM_OF_ROOT_NODES, (float)j / (float)TerrainBuffer::NUM_OF_ROOT_NODES);
				TerrainNode terrainNode = TerrainNode(m_buffer, location, 0, glm::vec2(i, j));
				m_children.push_back(terrainNode);

				float distanceFromCenterToSide = (terrainNode.GetGap() / 2.0f) * m_buffer->GetScaleXZ();
				glm::vec2 topLeftEdge = location + glm::vec2(-distanceFromCenterToSide, distanceFromCenterToSide);
				glm::vec2 topRightEdge = location + glm::vec2(distanceFromCenterToSide, distanceFromCenterToSide);
				glm::vec2 bottomLeftEdge = location + glm::vec2(-distanceFromCenterToSide, -distanceFromCenterToSide);
				glm::vec2 bottomRightEdge = location + glm::vec2(distanceFromCenterToSide, -distanceFromCenterToSide);
				float topLeftEdgeHeight = m_buffer->GetTerrainHeightCache(topLeftEdge.x, topLeftEdge.y);
				float topRightEdgeHeight = m_buffer->GetTerrainHeightCache(topRightEdge.x, topRightEdge.y);
				float bottomLeftEdgeHeight = m_buffer->GetTerrainHeightCache(bottomLeftEdge.x, bottomLeftEdge.y);
				float bottomRightEdgeHeight = m_buffer->GetTerrainHeightCache(bottomRightEdge.x, bottomRightEdge.y);

				glm::vec3 worldPosition = terrainNode.GetWorldPosition();
				glm::vec3 minAABB = glm::vec3(std::numeric_limits<float>::max());
				glm::vec3 maxAABB = glm::vec3(std::numeric_limits<float>::min());
				minAABB.x = std::min(minAABB.x, worldPosition.x - distanceFromCenterToSide);
				minAABB.y = std::min(minAABB.y, worldPosition.y);
				minAABB.y = std::min(minAABB.y, topLeftEdgeHeight);
				minAABB.y = std::min(minAABB.y, topRightEdgeHeight);
				minAABB.y = std::min(minAABB.y, bottomLeftEdgeHeight);
				minAABB.y = std::min(minAABB.y, bottomRightEdgeHeight);
				minAABB.z = std::min(minAABB.z, worldPosition.z - distanceFromCenterToSide);
				maxAABB.x = std::max(maxAABB.x, worldPosition.x + distanceFromCenterToSide);
				maxAABB.y = std::max(maxAABB.y, worldPosition.y);
				maxAABB.y = std::max(maxAABB.y, topLeftEdgeHeight);
				maxAABB.y = std::max(maxAABB.y, topRightEdgeHeight);
				maxAABB.y = std::max(maxAABB.y, bottomLeftEdgeHeight);
				maxAABB.y = std::max(maxAABB.y, bottomRightEdgeHeight);
				maxAABB.z = std::max(maxAABB.z, worldPosition.z + distanceFromCenterToSide);

				m_boundingBoxes.push_back(BoundingBox::Create(BoundingBoxType::AABB, minAABB, maxAABB));
			}
		}
	}

	void QuadtreeTerrain::OnUpdate(glm::vec3 cameraPosition)
	{
		for (uint32_t i = 0; i < m_children.size(); ++i)
		{
			m_children[i].Update(cameraPosition);
		}
	}

	void QuadtreeTerrain::Draw(glm::mat4 model, const Frustum& frustum)
	{
		for (uint32_t i = 0; i < m_children.size(); ++i)
		{
			if (m_boundingBoxes[i]->IsOnFrustum(frustum, {}))
			{
				m_children[i].Draw(frustum);
			}
		}
	}
}
