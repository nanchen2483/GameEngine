#pragma once

#include "Engine/Core/Core.h"

namespace Engine
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererId() const = 0;
		virtual const FramebufferSpecification GetSpecification() const = 0;
		static Ptr<Framebuffer> Create(const FramebufferSpecification spec);
	};
}