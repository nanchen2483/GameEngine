#include "enginepch.h"
#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Engine
{
	Ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLFramebuffer>(spec);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}