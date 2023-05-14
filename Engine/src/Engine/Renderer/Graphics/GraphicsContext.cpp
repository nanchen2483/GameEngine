#include "enginepch.h"
#include "GraphicsContext.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Engine
{
	Uniq<GraphicsContext> GraphicsContext::Create(IWindow* window)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreateUniq<OpenGLContext>((GLFWwindow*)window);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}