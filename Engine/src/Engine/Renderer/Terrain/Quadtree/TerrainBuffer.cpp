#include "enginepch.h"
#include "TerrainBuffer.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	TerrainBuffer::TerrainBuffer()
	{
		m_loadRange.reserve(NUM_OF_ROOT_NODES);
		m_loadMorphingArea.reserve(NUM_OF_ROOT_NODES);

		uint32_t index = 0;
		m_shader = Shader::Create("assets/shaders/TessellationTerrain.glsl");
		m_shader->Bind();
		m_shader->SetInt("uHeightMap", index++);
		m_shader->SetInt("uNormalMap", index++);
		m_shader->SetInt("uSplatMap", index++);
		for (uint32_t i = 0; i < m_materials.size(); ++i)
		{
			m_shader->SetInt("uMaterials[" + std::to_string(i) + "].diffuseMap", index++);
			m_shader->SetInt("uMaterials[" + std::to_string(i) + "].normalMap", index++);
			m_shader->SetInt("uMaterials[" + std::to_string(i) + "].heightMap", index++);
		}

		InitValue();
		SetHeightMap();
		SetNormalMap();
		SetSplatMap();
		SetMaterial();
	}

	void TerrainBuffer::InitValue()
	{
		m_scaleXZ = 6000.0f;
		m_scaleY = 600.0f;
		m_tessellationFactor = 600;
		m_tessellationSlope = 1.8f;
		m_tessellationShift = 0.1f;
		m_TBNRange = 200;

		AddLodRange(1750);
		AddLodRange(874);
		AddLodRange(386);
		AddLodRange(192);
		AddLodRange(100);
		AddLodRange(50);
		AddLodRange(0);
		AddLodRange(0);

		std::vector<float> vertices = std::vector<float>
		{
			0.0f,		0.0f,
			0.333f,		0.0f,
			0.666f,		0.0f,
			1.0f,		0.0f,

			0.0f,		0.333f,
			0.333f,		0.333f,
			0.666f,		0.333f,
			1.0f,		0.333f,

			0.0f,		0.666f,
			0.333f,		0.666f,
			0.666f,		0.666f,
			1.0f,		0.666f,

			0.0f,		1.0f,
			0.333f,		1.0f,
			0.666f,		1.0f,
			1.0f,		1.0f,
		};

		m_sizeOfPatch = vertices.size() / 2;
		m_vertexArray = VertexArray::Create();
		Ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vertices[0], sizeof(float) * vertices.size());
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float2 },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetNumOfPatchVertices(m_sizeOfPatch);
	}

	void TerrainBuffer::SetHeightMap()
	{
		m_heightMapTexture = Texture2D::Create("assets/textures/heights/hm0.bmp");
		m_heightMapDataBuffer = m_heightMapTexture->GetImageData();
	}

	void TerrainBuffer::SetNormalMap()
	{
		uint32_t N = m_heightMapTexture->GetWidth();
		m_normalMapTexture = Texture2D::Create(N, N, (int)(log(N) / log(2)), TextureFormatType::RGBA32);

		Ptr<Shader> normalShader = Shader::Create("assets/shaders/Compute/NormalMap.glsl");
		normalShader->Bind();
		normalShader->SetInt("uHeightMap", 0);
		normalShader->SetInt("uN", N);
		normalShader->SetFloat("uNormalStrength", 60.0f);
		m_heightMapTexture->Bind();
		m_normalMapTexture->BindImage();
		RendererCommand::Compute(N / 16, N / 16, 1);
	}

	void TerrainBuffer::SetSplatMap()
	{
		uint32_t N = m_heightMapTexture->GetWidth();
		m_splatMapTexture = Texture2D::Create(N, N, (int)(log(N) / log(2)), TextureFormatType::RGBA16);

		Ptr<Shader> splatShader = Shader::Create("assets/shaders/Compute/SplatMap.glsl");
		splatShader->Bind();
		splatShader->SetInt("uNormalMap", 0);
		splatShader->SetInt("uN", N);
		m_normalMapTexture->Bind();
		m_splatMapTexture->BindImage();
		RendererCommand::Compute(N / 16, N / 16, 1);
	}

	void TerrainBuffer::SetMaterial()
	{
		float displacementScaling[3]{ 0.8f, 1.0f, 2.0f };
		for (uint32_t i = 0; i < m_materials.size(); ++i)
		{
			m_materials[i].diffuseMap = Texture2D::Create("assets/textures/terrain/ground_" + std::to_string(i) + "_diffuse.jpg");
			m_materials[i].normalMap = Texture2D::Create("assets/textures/terrain/ground_" + std::to_string(i) + "_normal.jpg");
			m_materials[i].displacementMap = Texture2D::Create("assets/textures/terrain/ground_" + std::to_string(i) + "_displacement.jpg");
			m_materials[i].displacementScaling = displacementScaling[i];
			m_materials[i].horizontalScaling = 400.0f;
		}
	}

	void TerrainBuffer::AddLodRange(uint32_t lodRange)
	{
		m_loadRange.push_back(lodRange);
		uint32_t lodLevel = m_loadRange.size();
		m_loadMorphingArea.push_back(lodRange - GetMorphingArea(lodLevel));
	}

	uint32_t TerrainBuffer::GetMorphingArea(uint32_t lod)
	{
		return (uint32_t)(m_scaleXZ / (float)NUM_OF_ROOT_NODES) / std::pow(2, lod);
	}

	float TerrainBuffer::GetTerrainHeight(float x, float z)
	{
		float height = 0.0f;
		uint32_t width = m_heightMapTexture->GetWidth();

		glm::vec2 position = glm::vec2(x, z) + m_scaleXZ / 2.0f;
		position /= m_scaleXZ;
		glm::vec2 floor = glm::vec2(std::floor(position.x), std::floor(position.y));
		position -= floor;
		position *= width;

		int32_t x0 = (int32_t)std::floor(position.x);
		int32_t x1 = x0 + 1;
		int32_t z0 = (int32_t)std::floor(position.y);
		int32_t z1 = z0 + 1;

		float h0 = m_heightMapDataBuffer[width * z0 + x0];
		float h1 = m_heightMapDataBuffer[width * z0 + x1];
		float h2 = m_heightMapDataBuffer[width * z1 + x0];
		float h3 = m_heightMapDataBuffer[width * z1 + x1];

		float percentU = position.x - x0;
		float percentV = position.y - z0;

		float dU, dV;
		if (percentU > percentV)
		{
			dU = h1 - h0;
			dV = h3 - h1;
		}
		else
		{
			dU = h3 - h2;
			dV = h2 - h0;
		}

		height = h0 + (dU * percentU) + (dV * percentV);
		height *= m_scaleY;

		return height;
	}

	void TerrainBuffer::Draw(TerrainNodeData data)
	{
		m_shader->Bind();
		m_shader->SetMat4("uLocalMatrix", data.localTransform);
		m_shader->SetMat4("uWorldMatrix", data.worldTransform);

		m_shader->SetFloat("uScaleY", m_scaleY);
		m_shader->SetInt("uLod", data.lod);
		m_shader->SetFloat2("uIndex", data.index);
		m_shader->SetFloat("uGap", data.gap);
		m_shader->SetFloat2("uLocation", data.location);
		m_shader->SetInt("uTessellationFactor", m_tessellationFactor);
		m_shader->SetFloat("uTessellationSlope", m_tessellationSlope);
		m_shader->SetFloat("uTessellationShift", m_tessellationShift);
		for (uint32_t i = 0; i < m_loadMorphingArea.size(); ++i)
		{
			m_shader->SetInt("uLodMorphArea[" + std::to_string(i) + "]", m_loadMorphingArea[i]);
		}

		uint32_t slot = 0;
		m_heightMapTexture->Bind(slot++);
		m_normalMapTexture->Bind(slot++);
		m_splatMapTexture->Bind(slot++);
		for (uint32_t i = 0; i < m_materials.size(); ++i)
		{
			m_materials[i].diffuseMap->Bind(slot++);
			m_materials[i].normalMap->Bind(slot++);
			m_materials[i].displacementMap->Bind(slot++);
			m_shader->SetFloat("uMaterials[" + std::to_string(i) + "].heightScaling", m_materials[i].displacementScaling);
			m_shader->SetFloat("uMaterials[" + std::to_string(i) + "].horizontalScaling", m_materials[i].horizontalScaling);
		}

		m_shader->SetInt("uTBNRange", m_TBNRange);

		m_vertexArray->Bind();
		RendererCommand::DrawPatch(m_sizeOfPatch);
	}
}