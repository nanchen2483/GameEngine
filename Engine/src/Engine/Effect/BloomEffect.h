#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Framebuffer/IFramebuffer.h"
#include "Engine/Renderer/VertexArray/IVertexArray.h"
#include "Engine/Renderer/Shader/IShader.h"

namespace Engine
{
	class ENGINE_API BloomEffect
	{
	public:
		BloomEffect();
		uint32_t GetRendererId() const { return m_finalFramebuffer->GetColorAttachmentRendererId(); }
		void OnViewportResize(uint32_t width, uint32_t height);
		void OnUpdate(Ptr<IFramebuffer> sceneFramebuffer);
	private:
		Ptr<IShader> m_shaderBlur;
		Ptr<IShader> m_shaderFinal;
		Ptr<IVertexArray> m_vertexArray;
		Ptr<IFramebuffer> m_blurFramebuffers[2];
		Ptr<IFramebuffer> m_finalFramebuffer;
	};
}
