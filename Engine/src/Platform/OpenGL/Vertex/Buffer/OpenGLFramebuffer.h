#pragma once
#include "Engine/Renderer/Vertex/Buffer/Framebuffer.h"

namespace Engine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;
		virtual void BindDepthTexture(uint32_t slot) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		inline virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const override { ENGINE_CORE_ASSERT(index < m_colorAttachments.size(), ""); return m_colorAttachments[index]; };
		inline virtual const FramebufferSpecification GetSpecification() const override { return m_specification; };
	private:
		void Setup();
		uint32_t m_rendererId = 0;
		FramebufferSpecification m_specification;

		std::vector<FramebufferTextureSpecification> m_colorAttachmentSpecifications;
		FramebufferTextureSpecification m_depthAttachmentSpecification;

		std::vector<uint32_t> m_colorAttachments;
		uint32_t m_depthAttachment = 0;
	};
}


