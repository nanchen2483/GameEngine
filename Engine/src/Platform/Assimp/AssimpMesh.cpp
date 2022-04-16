#include "enginepch.h"
#include "AssimpMesh.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, AssimpMaterial material)
		: m_material(material), m_vertexArray(VertexArray::Create())
	{
		m_vertexArray->AddVertexBuffer(VertexBuffer::Create(&vertices[0], vertices.size()));
		m_vertexArray->SetIndexBuffer(IndexBuffer::Create(&indices[0], indices.size()));
	}

	void AssimpMesh::Draw()
	{
		m_material.Bind();

		// Draw mesh
		m_vertexArray->Bind();
		RendererCommand::DrawIndexed(m_vertexArray->GetNumOfIndices());
	}
}

