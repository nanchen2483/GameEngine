#include "enginepch.h"
#include "AssimpMesh.h"

#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{
	AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Material& material)
		: m_vertices(vertices), m_indices(indices), m_material(CreatePtr<Material>(material))
	{
	}

	Ptr<IVertexArray> AssimpMesh::GetVertexArray()
	{
		if (m_vertexArray == nullptr)
		{
			m_vertexArray = IVertexArray::Create();
			m_vertexArray->AddVertexBuffer(IVertexBuffer::Create(&(m_vertices)[0], m_vertices.size()));
			m_vertexArray->SetIndexBuffer(IIndexBuffer::Create(&(m_indices)[0], m_indices.size()));
			std::vector<Vertex>().swap(m_vertices);
			std::vector<uint32_t>().swap(m_indices);
		}

		return m_vertexArray;
	}
}
