#pragma once

#include <vector>
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Renderer/Vertex/VertexArray.h"
#include "Engine/Math/Transform.h"

namespace Engine
{
	struct TerrainNodeData
	{
		glm::vec2 location;
		uint32_t lod;
		glm::vec2 index;
		glm::vec3 worldPosition;
		float gap;

		Transform worldTransform;
		Transform localTransform;
	};

	struct TerrainMaterial
	{
		Ptr<Texture2D> diffuseMap;
		Ptr<Texture2D> normalMap;
		Ptr<Texture2D> displacementMap;
		float displacementScaling;
		float horizontalScaling;
	};

	class TerrainBuffer
	{
	public:
		TerrainBuffer();
		Ptr<Shader> GetShader() const { return m_shader; };
		float GetScaleY() const { return m_scaleY; }
		float GetScaleXZ() const { return m_scaleXZ; }
		int32_t GetLodMorphingArea(uint32_t lod) const { return m_loadMorphingArea[lod]; }
		uint32_t GetLodRange(uint32_t lod) const { return m_loadRange[lod]; }
		float GetTerrainHeight(float x, float z);
		void Draw(TerrainNodeData data);
	public:
		const static uint32_t NUM_OF_ROOT_NODES = 8;
	private:
		void InitValue();
		void SetHeightMap();
		void SetNormalMap();
		void SetSplatMap();
		void SetMaterial();
		void AddLodRange(uint32_t lodRange);
		uint32_t GetMorphingArea(uint32_t lod);

		Ptr<Shader> m_shader = nullptr;
		Ptr<Texture2D> m_heightMapTexture = nullptr;
		Ptr<Texture2D> m_normalMapTexture = nullptr;
		Ptr<Texture2D> m_splatMapTexture = nullptr;
		float* m_heightMapDataBuffer;

		float m_scaleY = 0.0f, m_scaleXZ = 0.0f;
		uint32_t m_tessellationFactor = 0;
		float m_tessellationSlope = 0.0f, m_tessellationShift = 0.0f;
		uint32_t m_TBNRange = 0;

		std::vector<uint32_t> m_loadRange = std::vector<uint32_t>();
		std::vector<int32_t> m_loadMorphingArea = std::vector<int32_t>();
		std::vector<TerrainMaterial> m_materials = std::vector<TerrainMaterial>(3);

		Ptr<VertexArray> m_vertexArray;
		uint32_t m_sizeOfPatch;
	};
}
