#pragma once
#include "TerrainBuffer.h"

namespace Engine
{
	class TerrainNode
	{
	public:
		TerrainNode(Ptr<TerrainBuffer> buffer, glm::vec2 location, uint32_t lod, glm::vec2 index);
		void Update(glm::vec3 position);
		void Draw();
	private:
		void AddChildNodes();
		void RemoveChildNodes();
		void ComputeWorldPosition();

		bool m_isLeaf;
		Ptr<TerrainBuffer> m_buffer = nullptr;
		TerrainNodeData m_data;
		std::vector<TerrainNode> m_children;
	};
}
