#include "enginepch.h"
#include "QuadtreeTerrain.h"

namespace Engine
{
	QuadtreeTerrain::QuadtreeTerrain()
	{
		Ptr<TerrainBuffer> buffer = CreatePtr<TerrainBuffer>();
		for (uint32_t i = 0; i < TerrainBuffer::NUM_OF_ROOT_NODES; ++i)
		{
			for (uint32_t j = 0; j < TerrainBuffer::NUM_OF_ROOT_NODES; ++j)
			{
				glm::vec2 location = glm::vec2((float)i / (float)TerrainBuffer::NUM_OF_ROOT_NODES, (float)j / (float)TerrainBuffer::NUM_OF_ROOT_NODES);
				m_children.push_back(TerrainNode(buffer, location, 0, glm::vec2(i, j)));
			}
		}
	}

	void QuadtreeTerrain::OnUpdate(glm::vec3 position)
	{
		for (uint32_t i = 0; i < m_children.size(); ++i)
		{
			m_children[i].Update(position);
		}
	}

	void QuadtreeTerrain::Draw()
	{
		for (uint32_t i = 0; i < m_children.size(); ++i)
		{
			m_children[i].Draw();
		}
	}
}
