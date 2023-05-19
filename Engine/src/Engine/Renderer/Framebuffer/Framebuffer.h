#pragma once
#include "Engine/Core/Base.h"
#include "Specifications/FramebufferSpecification.h"

namespace Engine
{
	class ENGINE_API Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void BindDepthTexture(uint32_t slot) = 0;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;
		virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification GetSpecification() const = 0;

		static Ptr<Framebuffer> Create(const FramebufferSpecification spec);
	};
}
