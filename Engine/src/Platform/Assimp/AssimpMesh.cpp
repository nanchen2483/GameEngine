#include "enginepch.h"
#include "AssimpMesh.h"

#include "Engine/Renderer/RendererCommand.h"

#include <glad/glad.h>

namespace Engine
{
	AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ptr<Texture>> textures)
		: m_textures(textures), m_vertexArray(VertexArray::Create())
	{
		m_vertexArray->AddVertexBuffer(VertexBuffer::Create(&vertices[0], vertices.size()));
		m_vertexArray->SetIndexBuffer(IndexBuffer::Create(&indices[0], indices.size()));
	}

	void AssimpMesh::Draw()
	{
		for (uint32_t i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->Bind(i);
		}

		// Draw mesh
		m_vertexArray->Bind();
		RendererCommand::DrawIndexed(m_vertexArray->GetNumOfIndices());
	}
}

