#pragma once
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Terrain/Terrain.h"
#include "TerrainVertex.h"

namespace Engine
{
	class TessellationTerrain : public Terrain
	{
	public:
		TessellationTerrain(std::string filePath, int32_t entityId = -1);
		TessellationTerrain(Ptr<Texture2D> heightMapTexture, int32_t entityId = -1);

		inline virtual std::string GetFilePath() const override { return m_heightMapTexture->GetFilePath(); }
		virtual float GetHeight(float x, float z) const override;
		virtual TerrainType GetType() const override { return TerrainType::Default; };

		virtual void OnUpdate(glm::vec3 position) override {};
		virtual void Draw(glm::mat4 model, const Frustum& frustum) override;
	private:
		std::vector<TerrainVertex> SetVertices(uint32_t numOfPoints, int32_t width, int32_t height);

		Ptr<Shader> m_shader;
		Ptr<VertexArray> m_vertexArray;
		Ptr<Texture2D> m_heightMapTexture;
		std::vector<float> m_heightMapDataBuffer;

		int32_t m_entityId = -1;
		const uint32_t m_numOfVerticesPerPatch = 4;
		const uint32_t m_numOfPoints = 20;
		const uint32_t m_numOfPatches = m_numOfPoints * m_numOfPoints * m_numOfVerticesPerPatch;
	};
}
