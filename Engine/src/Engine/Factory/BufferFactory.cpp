#include "enginepch.h"
#include "BufferFactory.h"

#include "Engine/Core/System/System.h"
#include "Platform/OpenGL/Buffer/OpenGLIndexBuffer.h"
#include "Platform/OpenGL/Buffer/OpenGLUniformBuffer.h"
#include "Platform/OpenGL/Buffer/OpenGLVertexBuffer.h"

namespace Engine
{
	Ptr<IVertexBuffer> BufferFactory::CreateVertexBuffer(uint32_t size)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(size);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IVertexBuffer> BufferFactory::CreateVertexBuffer(float* vertices, uint32_t numOfVertices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, numOfVertices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IVertexBuffer> BufferFactory::CreateVertexBuffer(Vertex* vertices, uint32_t numOfVertices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, numOfVertices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IVertexBuffer> BufferFactory::CreateVertexBuffer(const void* vertices, uint32_t size)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLVertexBuffer>(vertices, size);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IIndexBuffer> BufferFactory::CreateIndexBuffer(uint8_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IIndexBuffer> BufferFactory::CreateIndexBuffer(uint16_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IIndexBuffer> BufferFactory::CreateIndexBuffer(uint32_t* indices, uint32_t numOfIndices)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLIndexBuffer>(indices, numOfIndices);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}

	Ptr<IUniformBuffer> BufferFactory::CreateUniformBuffer(UniformBufferBindingPoint bindingPoint, const BufferLayout layout, uint32_t numOfUniforms)
	{
		switch (System::GetGraphicsApiType())
		{
		case GraphicsApiType::None:			ENGINE_CORE_ASSERT(false, "GraphicsAPI::None is not supported"); return nullptr;
		case GraphicsApiType::OpenGL:		return CreatePtr<OpenGLUniformBuffer>(bindingPoint, layout, numOfUniforms);
		}

		ENGINE_CORE_ASSERT(false, "Unknown GraphicsAPI");
		return nullptr;
	}
}
