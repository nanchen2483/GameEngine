#include "enginepch.h"
#include "GraphicsFactory.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#include "Platform/OpenGL/OpenGraphicsLibrary.h"

namespace Engine
{
	Uniq<IGraphicsContext> GraphicsFactory::CreateContext(void* window)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreateUniq<OpenGLContext>((GLFWwindow*)window);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Uniq<IGraphicsAPI> CreateAPI()
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreateUniq<OpenGLGraphicsAPI>();
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}


	IGraphicsAPI& GraphicsFactory::GetAPIInstance()
	{
		static Uniq<IGraphicsAPI> s_instance = CreateAPI();
		return *s_instance;
	}

	Uniq<IGraphicsLibrary> CreateLibrary()
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreateUniq<OpenGraphicsLibrary>();
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	IGraphicsLibrary& GraphicsFactory::GetLibraryInstance()
	{
		static Uniq<IGraphicsLibrary> s_instance = CreateLibrary();
		return *s_instance;
	}

	WindowUserData* GraphicsFactory::GetWindowUserDataPointerStatic(void* window)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return OpenGraphicsLibrary::GetWindowUserDataPointerStatic(window);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
