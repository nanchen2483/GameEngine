#pragma once

#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"
#include "Engine/Renderer/Vertex/Buffer/Framebuffer.h"
#include "Engine/Renderer/Vertex/VertexArray.h"

namespace Engine
{
	class ShadowDebug
	{
	public:
		ShadowDebug();
		void Draw();
		uint32_t GetShadowLayer(uint32_t layer) const;
	private:
		Ptr<Shader> m_shader;
		Ptr<VertexArray> m_vertexArray;
		Ptr<Framebuffer> m_framebuffer;

		const ShadowBoxInfo m_shadowInfo;
	};
}
