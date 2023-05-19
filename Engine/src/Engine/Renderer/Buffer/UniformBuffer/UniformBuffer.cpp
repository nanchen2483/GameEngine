#include "enginepch.h"
#include "UniformBuffer.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Buffer/OpenGLUniformBuffer.h"

namespace Engine
{
	Ptr<UniformBuffer> UniformBuffer::Create(uint32_t index, const BufferLayout layout)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLUniformBuffer>(index, layout);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
