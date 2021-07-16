#pragma once

#include "Engine/Core/Base.h"

namespace Engine
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8,
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format)
			: textureFormat(format) {}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::vector<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		FramebufferAttachmentSpecification attachments;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const = 0;
		virtual const FramebufferSpecification GetSpecification() const = 0;
		static Ptr<Framebuffer> Create(const FramebufferSpecification spec);
	};
}