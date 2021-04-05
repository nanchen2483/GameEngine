#include "enginepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine
{
	Ptr<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexBuffer>(size);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	Ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}

	Ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
		return nullptr;
	}
}
