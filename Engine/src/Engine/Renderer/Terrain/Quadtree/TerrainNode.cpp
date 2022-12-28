#include "enginepch.h"
#include "TerrainNode.h"

namespace Engine
{
	TerrainNode::TerrainNode(Ptr<TerrainBuffer> buffer, glm::vec2 location, uint32_t lod, glm::vec2 index)
		: m_isLeaf(true), m_buffer(buffer), m_location(location), m_lod(lod), m_index(index)
	{
		m_gap = 1.0f / (float)(TerrainBuffer::NUM_OF_ROOT_NODES * std::pow(2, lod));

		m_localTransform.scale = glm::vec3(m_gap, 0.0f, m_gap);
		m_localTransform.translation = glm::vec3(m_location.x, 0.0f, m_location.y);

		ComputeWorldPosition();
	}
	
	void TerrainNode::ComputeWorldPosition()
	{
		glm::vec2 location = (m_location + glm::vec2(m_gap / 2.0f)) * glm::vec2(m_buffer->GetScaleXZ()) - glm::vec2(m_buffer->GetScaleXZ() / 2.0f);
		float centerHeight = m_buffer->GetTerrainHeightCache(location.x, location.y);
		m_worldPosition = glm::vec3(location.x, centerHeight, location.y);
	}

	void TerrainNode::Update(glm::vec3 cameraPosition)
	{
		float distance = glm::length(cameraPosition - m_worldPosition);
		if (distance < m_buffer->GetLodRange(m_lod))
		{
			AddChildNodes();
		}
		else
		{
			RemoveChildNodes();
		}

		if (!m_isLeaf)
		{
			for (uint32_t i = 0; i < m_children.size(); ++i)
			{
				m_children[i].Update(cameraPosition);
			}
		}
	}
	
	void TerrainNode::AddChildNodes()
	{
		if (m_isLeaf)
		{
			m_isLeaf = false;
		}

		if (m_children.size() == 0)
		{
			for (uint32_t i = 0; i < 2; ++i)
			{
				for (uint32_t j = 0; j < 2; ++j)
				{
					glm::vec2 location = m_location + glm::vec2((float)i * m_gap / 2.0f, (float)j * m_gap / 2.0f);
					m_children.push_back(TerrainNode(m_buffer, location, m_lod + 1, glm::vec2(i, j)));
				}
			}
		}
	}
	
	void TerrainNode::RemoveChildNodes()
	{
		if (!m_isLeaf)
		{
			m_isLeaf = true;
		}

		if (m_children.size() != 0)
		{
			m_children.clear();
		}
	}

	void TerrainNode::Draw(const Frustum& frustum)
	{
		if (m_isLeaf)
		{
			m_buffer->Draw(m_localTransform, m_location, m_lod, m_index, m_gap);
		}
		else
		{
			for (uint32_t i = 0; i < m_children.size(); ++i)
			{
				m_children[i].Draw(frustum);
			}
		}
	}
}
