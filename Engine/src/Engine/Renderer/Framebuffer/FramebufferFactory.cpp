#include "enginepch.h"
#include "FramebufferFactory.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Framebuffer/OpenGLFramebuffer.h"

namespace Engine
{
	Ptr<IFramebuffer> FramebufferFactory::Create(const FramebufferSpecification spec)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLFramebuffer>(spec);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
