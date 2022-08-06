#pragma once
#include "TerrainNode.h"

namespace Engine
{
	class QuadtreeTerrain
	{
	public:
		QuadtreeTerrain();
		void OnUpdate(glm::vec3 position);
		void Draw();
	private:
		std::vector<TerrainNode> m_children;
	};
}
