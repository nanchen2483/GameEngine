#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Buffer/IVertexArray.h"
#include "Engine/Renderer/Shader/IShader.h"
#include "Engine/Renderer/Texture/ITexture.h"

namespace Engine
{
	struct TerrainMaterial
	{
		Ptr<ITexture2D> diffuseMap;
		Ptr<ITexture2D> normalMap;
		Ptr<ITexture2D> displacementMap;
		float displacementScaling;
		float horizontalScaling;
	};

	class TerrainBuffer
	{
	public:
		TerrainBuffer(Ptr<ITexture2D> heightMapTexture, int32_t entityId);
		Ptr<IShader> GetShader() const { return m_shader; };
		float GetScaleY() const { return m_scaleY; }
		float GetScaleXZ() const { return m_scaleXZ; }
		std::string GetHeightMapPath() const { return m_heightMapTexture->GetFilePath(); }
		int32_t GetLodMorphingArea(uint32_t lod) const { return m_loadMorphingArea[lod]; }
		uint32_t GetLodRange(uint32_t lod) const { return m_loadRange[lod]; }
		float GetTerrainHeightCache(float x, float z);
		float GetTerrainHeight(float x, float z) const;
		void Draw(glm::mat4 localTransform, glm::vec2 location, uint32_t lod, glm::vec2 index, float gap);
	public:
		const static uint32_t NUM_OF_ROOT_NODES = 8;
	private:
		void InitValue();
		void SetNormalMap();
		void SetSplatMap();
		void SetMaterial();
		void AddLodRange(uint32_t lodRange);
		uint32_t GetMorphingArea(uint32_t lod);

		Ptr<IShader> m_shader = nullptr;
		Ptr<ITexture2D> m_heightMapTexture = nullptr;
		Ptr<ITexture2D> m_normalMapTexture = nullptr;
		Ptr<ITexture2D> m_splatMapTexture = nullptr;
		std::vector<float> m_heightMapDataBuffer;
		Ptr<IUniformBuffer> m_wordTransform = nullptr;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, float>> m_heightCache;

		int32_t m_entityId;
		float m_scaleY = 0.0f, m_scaleXZ = 0.0f;
		uint32_t m_tessellationFactor = 0;
		float m_tessellationSlope = 0.0f, m_tessellationShift = 0.0f;
		uint32_t m_TBNRange = 0;

		std::vector<uint32_t> m_loadRange = std::vector<uint32_t>();
		std::vector<int32_t> m_loadMorphingArea = std::vector<int32_t>();
		std::vector<TerrainMaterial> m_materials = std::vector<TerrainMaterial>(3);

		Ptr<IVertexArray> m_vertexArray;
		uint32_t m_sizeOfPatch;
	};
}
