#include "enginepch.h"
#include "TessellationTerrain.h"
#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Library/TextureLibrary.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	TessellationTerrain::TessellationTerrain(std::string filePath, int32_t entityId)
		: TessellationTerrain(TextureLibrary::Load(filePath, TextureType::Height, false), entityId)
	{
	}

	TessellationTerrain::TessellationTerrain(Ptr<Texture2D> heightMapTexture, int32_t entityId)
		: m_heightMapTexture(heightMapTexture), m_entityId(entityId)
	{
		m_heightMapDataBuffer = m_heightMapTexture->GetData();

		m_vertexArray = VertexArray::Create();
		std::vector<TerrainVertex> vertices = SetVertices(m_numOfPoints, m_heightMapTexture->GetWidth(), m_heightMapTexture->GetHeight());
		Ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vertices[0], sizeof(TerrainVertex) * vertices.size());
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
			{ ShaderDataType::Float2 },
			{ ShaderDataType::Int },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetNumOfPatchVertices(m_numOfVerticesPerPatch);

		m_shader = ShaderLibrary::Load("assets/shaders/Terrian.glsl");
		m_shader->Bind();
		m_shader->SetInt("uHeightMap", 0);
	}

	float TessellationTerrain::GetHeight(float x, float z) const
	{
		glm::vec3 terrianSize = glm::vec3(m_heightMapTexture->GetWidth(), 0, m_heightMapTexture->GetHeight());

		glm::vec3 position = glm::vec3(x, 0, z) + terrianSize / 2.0f;
		position /= terrianSize;
		glm::vec3 floor(std::floor(position.x), 0, std::floor(position.z));
		position -= floor;
		position *= terrianSize - glm::vec3(1);

		int32_t x0 = (int32_t)std::floor(position.x);
		int32_t x1 = x0 + 1;
		if (x1 >= terrianSize.x || position.x == x0)
		{
			x1 = x0;
		}

		int32_t z0 = (int32_t)std::floor(position.z);
		int32_t z1 = z0 + 1;
		if (z1 >= terrianSize.z || position.z == z0)
		{
			z1 = z0;
		}

		float h0 = m_heightMapDataBuffer[terrianSize.x * z0 + x0];
		float h1 = m_heightMapDataBuffer[terrianSize.x * z0 + x1];
		float h2 = m_heightMapDataBuffer[terrianSize.x * z1 + x0];
		float h3 = m_heightMapDataBuffer[terrianSize.x * z1 + x1];

		float percentU = position.x - x0;
		float percentV = position.z - z0;

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

		float y = h0 + (dU * percentU) + (dV * percentV);

		return y * 64.0f - 16.0f;
	}
	
	std::vector<TerrainVertex> TessellationTerrain::SetVertices(uint32_t numOfPoints, int32_t width, int32_t height)
	{
		std::vector<TerrainVertex> vertices;
		vertices.reserve(numOfPoints * numOfPoints * 4);
		TerrainVertex vertex;
		for (uint32_t i = 0; i < numOfPoints; i++)
		{
			for (uint32_t j = 0; j < numOfPoints; j++)
			{
				vertex.position = glm::vec3(-width / 2.0f + width * i / (float)numOfPoints, 0.0f, -height / 2.0f + height * j / (float)numOfPoints);
				vertex.texCoord = glm::vec2(i / (float)numOfPoints, j / (float)numOfPoints);
				vertex.entityId = m_entityId;
				vertices.push_back(vertex);

				vertex.position = glm::vec3(-width / 2.0f + width * (i + 1) / (float)numOfPoints, 0.0f, -height / 2.0f + height * j / (float)numOfPoints);
				vertex.texCoord = glm::vec2((i + 1) / (float)numOfPoints, j / (float)numOfPoints);
				vertex.entityId = m_entityId;
				vertices.push_back(vertex);

				vertex.position = glm::vec3(-width / 2.0f + width * i / (float)numOfPoints, 0.0f, -height / 2.0f + height * (j + 1) / (float)numOfPoints);
				vertex.texCoord = glm::vec2(i / (float)numOfPoints, (j + 1) / (float)numOfPoints);
				vertex.entityId = m_entityId;
				vertices.push_back(vertex);

				vertex.position = glm::vec3(-width / 2.0f + width * (i + 1) / (float)numOfPoints, 0.0f, -height / 2.0f + height * (j + 1) / (float)numOfPoints);
				vertex.texCoord = glm::vec2((i + 1) / (float)numOfPoints, (j + 1) / (float)numOfPoints);
				vertex.entityId = m_entityId;
				vertices.push_back(vertex);
			}
		}

		return vertices;
	}

	void TessellationTerrain::Draw(glm::mat4 model, const Frustum& frustum)
	{
		m_shader->Bind();
		m_shader->SetMat4("uModel", model);
		m_vertexArray->Bind();
		m_heightMapTexture->Bind();
		RendererCommand::DrawPatch(m_numOfPatches);
	}
}
