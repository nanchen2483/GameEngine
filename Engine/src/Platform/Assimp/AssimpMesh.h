#pragma once

#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Texture/Texture.h"
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
		AssimpMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Ptr<Texture>> textures);
		void Draw();
	private:
		Ptr<VertexArray> m_vertexArray;
		std::vector<Ptr<Texture>> m_textures;
	};
}
