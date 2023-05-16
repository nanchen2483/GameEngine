#include "enginepch.h"
#include "IGraphicsLibrary.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/OpenGraphicsLibrary.h"

namespace Engine
{
	WindowUserData* IGraphicsLibrary::GetWindowUserDataPointerStatic(void* window)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return OpenGraphicsLibrary::GetWindowUserDataPointerStatic(window);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	IGraphicsLibrary* Create()
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return new OpenGraphicsLibrary();
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	IGraphicsLibrary& IGraphicsLibrary::GetInstance()
	{
		static IGraphicsLibrary* s_instance = Create();
		return *s_instance;
	}
}
