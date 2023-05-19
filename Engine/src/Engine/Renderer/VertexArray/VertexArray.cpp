#include "enginepch.h"
#include "VertexArray.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/VertexArray/OpenGLVertexArray.h"

namespace Engine
{
	Ptr<VertexArray> VertexArray::Create()
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreateUniq<OpenGLVertexArray>();
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
