#pragma once
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Model/Vertex.h"
#include "Engine/Renderer/Material/Material.h"

namespace Engine
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material);

		Ptr<VertexArray> GetVertexArray();
		Ptr<Material> GetMaterial() const { return m_material; }
	private:
		Ptr<VertexArray> m_vertexArray;
		Ptr<Material> m_material;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
	};
}
