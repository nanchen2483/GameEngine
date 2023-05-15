#include "enginepch.h"
#include "GraphicsAPI.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

namespace Engine
{
	Uniq<GraphicsAPI> GraphicsAPI::Create()
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreateUniq<OpenGLGraphicsAPI>();
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
