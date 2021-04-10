#pragma once

#include "Engine/Renderer/Framebuffer.h"

namespace Engine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		inline virtual uint32_t GetColorAttachmentRendererId() const override { return m_colorAttachment; };

		inline virtual const FramebufferSpecification GetSpecification() const override { return m_specification; };
	private:
		uint32_t m_rendererId;
		uint32_t m_colorAttachment, m_depthAttachment;
		FramebufferSpecification m_specification;
	};
}


