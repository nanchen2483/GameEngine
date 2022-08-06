#include "enginepch.h"
#include "TerrainNode.h"

namespace Engine
{
	TerrainNode::TerrainNode(Ptr<TerrainBuffer> buffer, glm::vec2 location, uint32_t lod, glm::vec2 index)
		: m_isLeaf(true), m_buffer(buffer)
	{
		m_data.location = location;
		m_data.lod = lod;
		m_data.index = index;
		m_data.gap = 1.0f / (float)(TerrainBuffer::NUM_OF_ROOT_NODES * std::pow(2, lod));

		m_data.localTransform.scale = glm::vec3(m_data.gap, 0.0f, m_data.gap);
		m_data.localTransform.translation = glm::vec3(m_data.location.x, 0.0f, m_data.location.y);

		m_data.worldTransform.scale = glm::vec3(buffer->GetScaleXZ(), buffer->GetScaleY(), buffer->GetScaleXZ());
		m_data.worldTransform.translation = glm::vec3(-buffer->GetScaleXZ() / 2.0f, 0.0f, -buffer->GetScaleXZ() / 2.0f);

		ComputeWorldPosition();
	}
	
	void TerrainNode::Update(glm::vec3 position)
	{
		float distance = glm::length(position - m_data.worldPosition);
		if (distance < m_buffer->GetLodRange(m_data.lod))
		{
			AddChildNodes();
		}
		else
		{
			RemoveChildNodes();
		}

		for (uint32_t i = 0; i < m_children.size(); ++i)
		{
			m_children[i].Update(position);
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
					glm::vec2 location = m_data.location + glm::vec2((float)i * m_data.gap / 2.0f, (float)j * m_data.gap / 2.0f);
					m_children.push_back(TerrainNode(m_buffer, location, m_data.lod + 1, glm::vec2(i, j)));
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
	
	void TerrainNode::ComputeWorldPosition()
	{
		glm::vec2 location = (m_data.location + glm::vec2(m_data.gap / 2.0f)) * glm::vec2(m_buffer->GetScaleXZ()) - glm::vec2(m_buffer->GetScaleXZ() / 2.0f);
		float height = m_buffer->GetTerrainHeight(location.x, location.y);
		m_data.worldPosition = glm::vec3(location.x, height, location.y);
	}

	void TerrainNode::Draw()
	{
		if (m_isLeaf)
		{
			m_buffer->Draw(m_data);
		}
		else
		{
			for (uint32_t i = 0; i < m_children.size(); ++i)
			{
				m_children[i].Draw();
			}
		}
	}
}
