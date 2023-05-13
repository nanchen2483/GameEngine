#include "enginepch.h"
#include "Buffer.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Vertex/Buffer/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/Vertex/Buffer/OpenGLUniformBuffer.h"
#include "Platform/OpenGL/Vertex/Buffer/OpenGLVertexBuffer.h"

namespace Engine
{
	Ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(size);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t numOfVertices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, numOfVertices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<VertexBuffer> VertexBuffer::Create(Vertex* vertices, uint32_t numOfVertices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, numOfVertices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<VertexBuffer> VertexBuffer::Create(const void* vertices, uint32_t size)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, size);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

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
