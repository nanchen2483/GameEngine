#pragma once
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Renderer/Vertex/VertexArray.h"

#include <string>
#include <vector>

namespace Engine
{
	class Skybox
	{
	public:
		Skybox(const std::vector<Ptr<Image>> faces);
		Skybox(Ptr<Texture3D> texture);

		void SetTexture(Ptr<Texture3D> texture) { m_texture = texture; }
		void Draw();
	private:
		Ptr<Shader> m_shader;
		Ptr<Texture3D> m_texture;
		Ptr<VertexArray> m_vertexArray;
	};
}
