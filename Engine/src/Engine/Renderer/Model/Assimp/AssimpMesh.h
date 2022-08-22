#pragma once
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Model/Vertex.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Material.h"

namespace Engine
{
	class AssimpMesh
	{
	public:
		AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, const Ptr<Material> material, Ptr<TextureMap> textureMap);

		void Draw();
	private:
		Ptr<VertexArray> m_vertexArray;
		Ptr<std::vector<Vertex>> m_vertices;
		Ptr<std::vector<uint32_t>> m_indices;
		const Ptr<Material> m_material;
		const Ptr<TextureMap> m_textureMap;
	};
}
