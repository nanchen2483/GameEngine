#pragma once
#include "Engine/Renderer/Buffer/IVertexArray.h"
#include "Engine/Renderer/Shader/IShader.h"
#include "Engine/Renderer/Texture/ITexture.h"

#include <string>
#include <vector>

namespace Engine
{
	class ENGINE_API Skybox
	{
	public:
		Skybox(const std::vector<Ptr<Image>> faces);
		Skybox(Ptr<ITexture3D> texture);

		Ptr<ITexture3D> GetTexture() const { return m_texture; }
		void SetTexture(Ptr<ITexture3D> texture) { m_texture = texture; }
		void Draw();
	private:
		Ptr<IShader> m_shader;
		Ptr<ITexture3D> m_texture;
		Ptr<IVertexArray> m_vertexArray;
	};
}
