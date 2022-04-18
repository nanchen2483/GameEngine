#include "enginepch.h"
#include "AssimpMesh.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Ptr<Material> material, Ptr<TextureMap> textureMap)
		:	m_vertices(CreatePtr<std::vector<Vertex>>(vertices)),
			m_indices(CreatePtr<std::vector<uint32_t>>(indices)),
			m_material(material),
			m_textureMap(textureMap)
	{
	}

	void AssimpMesh::Draw()
	{
		if (m_vertexArray == nullptr)
		{
			m_vertexArray = VertexArray::Create();
			m_vertexArray->AddVertexBuffer(VertexBuffer::Create(&(*m_vertices)[0], m_vertices->size()));
			m_vertexArray->SetIndexBuffer(IndexBuffer::Create(&(*m_indices)[0], m_indices->size()));
			m_vertices = nullptr;
			m_indices = nullptr;

			m_material->Setup(m_textureMap);
		}

		m_material->Bind();
		m_vertexArray->Bind();
		RendererCommand::DrawIndexed(m_vertexArray->GetNumOfIndices());
	}
}

