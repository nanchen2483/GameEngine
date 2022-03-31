#pragma once

#include "Engine/Renderer/Shader.h"
#include <Engine/Renderer/Texture.h>
#include <Engine/Renderer/Vertex.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Engine::ModelData
{
	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Ptr<Texture>> textures);
		void Draw();

	private:
		void SetupMesh();
		
		uint32_t m_VAO;
		uint32_t m_VBO;
		uint32_t m_EBO;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<Ptr<Texture>> m_textures;
	};
}
