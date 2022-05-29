#include "enginepch.h"
#include "Terrian.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	Terrian::Terrian(std::string filePath, int32_t entityId)
		: Terrian(Texture2D::Create(filePath, TextureType::Height, false), entityId)
	{
	}

	Terrian::Terrian(Ptr<Texture2D> texture, int32_t entityId)
		: m_texture(texture), m_entityId(entityId)
	{
		m_vertexArray = VertexArray::Create();
		std::vector<TerrainVertex> vertices = SetVertices(m_numOfPoints, m_texture->GetWidth(), m_texture->GetHeight());
		Ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vertices[0], sizeof(TerrainVertex) * vertices.size());
		vertexBuffer->SetLayout({
			{ ShaderDataType::Float3 },
			{ ShaderDataType::Float2 },
			{ ShaderDataType::Int },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetNumOfPatchVertices(m_numOfVerticesPerPatch);

		m_shader = Shader::Create("assets/shaders/Terrian.glsl");
		m_shader->Bind();
		m_shader->SetInt("uHeightMap", 0);
	}
	
	std::vector<TerrainVertex> Terrian::SetVertices(uint32_t numOfPoints, int32_t width, int32_t height)
	{
		std::vector<TerrainVertex> vertices;
		vertices.reserve(numOfPoints * numOfPoints * 4);
		TerrainVertex vertex;
		for (uint32_t i = 0; i < numOfPoints; i++)
		{
			for (uint32_t j = 0; j < numOfPoints; j++)
			{
				vertex.position = glm::vec3(- width / 2.0f + width * i / (float)numOfPoints, 0.0f, -height / 2.0f + height * j / (float)numOfPoints);
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

	void Terrian::Draw(glm::mat4 model)
	{
		m_shader->Bind();
		m_shader->SetMat4("uModel", model);
		m_vertexArray->Bind();
		m_texture->Bind();
		RendererCommand::DrawPatch(m_numOfPatches);
	}
}
