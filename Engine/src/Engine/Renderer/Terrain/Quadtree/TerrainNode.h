#pragma once
#include "TerrainBuffer.h"
#include "Engine/Physics/BoundingVolume/BoundingVolume.h"

namespace Engine
{
	class TerrainNode
	{
	public:
		TerrainNode(Ptr<TerrainBuffer> buffer, glm::vec2 location, uint32_t lod, glm::vec2 index);

		glm::vec3 GetWorldPosition() const { return m_worldPosition; };
		float GetGap() const { return m_gap; }

		void Update(glm::vec3 position);
		void Draw(const Frustum& frustum);
	private:
		void AddChildNodes();
		void RemoveChildNodes();
		void ComputeWorldPosition();

		bool m_isLeaf;
		float m_gap;
		uint32_t m_lod;
		glm::vec2 m_index;
		glm::vec2 m_location;
		glm::vec3 m_worldPosition;
		Transform m_localTransform;

		Ptr<TerrainBuffer> m_buffer = nullptr;
		std::vector<TerrainNode> m_children;
	};
}
