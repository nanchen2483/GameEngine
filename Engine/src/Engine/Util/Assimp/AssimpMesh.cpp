#include "enginepch.h"
#include "AssimpMesh.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Material& material)
		: m_vertices(vertices), m_indices(indices), m_material(CreateUniq<Material>(material))
	{
	}

	void AssimpMesh::Draw()
	{
		if (m_vertexArray == nullptr)
		{
			m_vertexArray = VertexArray::Create();
			m_vertexArray->AddVertexBuffer(VertexBuffer::Create(&(m_vertices)[0], m_vertices.size()));
			m_vertexArray->SetIndexBuffer(IndexBuffer::Create(&(m_indices)[0], m_indices.size()));
			m_vertices = {};
			m_indices = {};
		}

		m_material->Bind();
		m_vertexArray->Bind();
		RendererCommand::DrawUint32Indexed(m_vertexArray->GetNumOfIndices());
	}
}
