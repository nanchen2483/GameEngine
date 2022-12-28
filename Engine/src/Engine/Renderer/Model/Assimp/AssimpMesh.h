#pragma once
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Model/Vertex.h"
#include "Material.h"

namespace Engine
{
	class AssimpMesh
	{
	public:
		AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Material& material);

		void Draw();
	private:
		Uniq<VertexArray> m_vertexArray;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		Uniq<Material> m_material;
	};
}
