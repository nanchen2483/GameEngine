#pragma once
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Renderer/Vertex/VertexArray.h"
#include "TerrainVertex.h"
#include "Quadtree/QuadtreeTerrain.h"

namespace Engine
{
	class Terrain
	{
	public:
		Terrain(std::string filePath, int32_t entityId = -1);
		Terrain(Ptr<Texture2D> texture, int32_t entityId = -1);

		std::string GetFilePath() const { return m_texture->GetFilePath(); }
		void Draw(glm::mat4 model);
	private:
		std::vector<TerrainVertex> SetVertices(uint32_t numOfPoints, int32_t width, int32_t height);

		Ptr<Shader> m_shader;
		Ptr<Texture2D> m_texture;
		Ptr<VertexArray> m_vertexArray;

		int32_t m_entityId = -1;
		const uint32_t m_numOfVerticesPerPatch = 4;
		const uint32_t m_numOfPoints = 20;
		const uint32_t m_numOfPatches = m_numOfPoints * m_numOfPoints * m_numOfVerticesPerPatch;
	};
}
