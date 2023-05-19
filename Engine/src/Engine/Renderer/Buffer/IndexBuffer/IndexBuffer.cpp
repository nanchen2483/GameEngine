#include "enginepch.h"
#include "IndexBuffer.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Buffer/OpenGLIndexBuffer.h"

namespace Engine
{
	Ptr<IndexBuffer> IndexBuffer::Create(uint8_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IndexBuffer> IndexBuffer::Create(uint16_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
