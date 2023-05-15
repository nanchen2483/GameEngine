#include "enginepch.h"
#include "IBuffer.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Vertex/Buffer/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/Vertex/Buffer/OpenGLUniformBuffer.h"
#include "Platform/OpenGL/Vertex/Buffer/OpenGLVertexBuffer.h"

namespace Engine
{
	Ptr<IVertexBuffer> IVertexBuffer::Create(uint32_t size)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(size);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IVertexBuffer> IVertexBuffer::Create(float* vertices, uint32_t numOfVertices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, numOfVertices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IVertexBuffer> IVertexBuffer::Create(Vertex* vertices, uint32_t numOfVertices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, numOfVertices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IVertexBuffer> IVertexBuffer::Create(const void* vertices, uint32_t size)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, size);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IIndexBuffer> IIndexBuffer::Create(uint8_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IIndexBuffer> IIndexBuffer::Create(uint16_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IIndexBuffer> IIndexBuffer::Create(uint32_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
	
	Ptr<IUniformBuffer> IUniformBuffer::Create(uint32_t index, const BufferLayout layout)
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
