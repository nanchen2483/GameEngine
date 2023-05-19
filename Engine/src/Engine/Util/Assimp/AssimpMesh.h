#pragma once
#include "Engine/Renderer/Model/Mesh/IMesh.h"

namespace Engine
{
	class AssimpMesh : public IMesh
	{
	public:
		AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Material& material);

		virtual Ptr<VertexArray> GetVertexArray() override;
		virtual Ptr<Material> GetMaterial() override { return m_material; }
	private:
		Ptr<VertexArray> m_vertexArray;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		Ptr<Material> m_material;
	};
}
