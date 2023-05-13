#pragma once
#include "Engine/Renderer/Buffer/VertexArray.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <string>
#include <vector>

namespace Engine
{
	class ENGINE_API Skybox
	{
	public:
		Skybox(const std::vector<Ptr<Image>> faces);
		Skybox(Ptr<Texture3D> texture);

		Ptr<Texture3D> GetTexture() const { return m_texture; }
		void SetTexture(Ptr<Texture3D> texture) { m_texture = texture; }
		void Draw();
	private:
		Ptr<Shader> m_shader;
		Ptr<Texture3D> m_texture;
		Ptr<VertexArray> m_vertexArray;
	};
}
