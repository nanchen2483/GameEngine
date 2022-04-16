#pragma once

#include "AssimpMaterial.h"
#include "Engine/Renderer/Vertex/Vertex.h"
#include "Engine/Renderer/Vertex/VertexArray.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Engine
{
	class AssimpMesh
	{
	public:
		AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, AssimpMaterial material);
		void Draw();
	private:
		const Ptr<VertexArray> m_vertexArray;
		AssimpMaterial m_material;
	};
}
