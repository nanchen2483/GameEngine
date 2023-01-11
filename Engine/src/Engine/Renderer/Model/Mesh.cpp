#include "Mesh.h"
namespace Engine
{
	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material)
		: m_vertices(vertices), m_indices(indices), m_material(CreatePtr<Material>(material))
	{
	}

	Ptr<VertexArray> Mesh::GetVertexArray()
	{
		if (m_vertexArray == nullptr)
		{
			m_vertexArray = VertexArray::Create();
			m_vertexArray->AddVertexBuffer(VertexBuffer::Create(&(m_vertices)[0], m_vertices.size()));
			m_vertexArray->SetIndexBuffer(IndexBuffer::Create(&(m_indices)[0], m_indices.size()));
			m_vertices = {};
			m_indices = {};
		}

		return m_vertexArray;
	}
}
