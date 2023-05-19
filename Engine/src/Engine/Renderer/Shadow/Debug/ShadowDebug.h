#pragma once
#include "Engine/Renderer/Framebuffer/IFramebuffer.h"
#include "Engine/Renderer/VertexArray/IVertexArray.h"
#include "Engine/Renderer/Shader/IShader.h"
#include "Engine/Renderer/Shadow/ShadowBox.h"

namespace Engine
{
	class ENGINE_API ShadowDebug
	{
	public:
		ShadowDebug();
		void Draw();
		uint32_t GetShadowLayer(uint32_t layer) const;
	private:
		Ptr<IShader> m_shader;
		Ptr<IVertexArray> m_vertexArray;
		Ptr<IFramebuffer> m_framebuffer;

		const ShadowBoxInfo m_shadowInfo;
	};
}
