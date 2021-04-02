#include "enginepch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine
{
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLVertexBuffer(vertices, size);
        default:
            break;
        }
        ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, size_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::None:
            ENGINE_CORE_ASSERT(false, "RendererAPI::None is not supported");
            return nullptr;
        case RendererAPI::OpenGL:
            return new OpenGLIndexBuffer(indices, count);
        default:
            break;
        }
        ENGINE_CORE_ASSERT(false, "Unknow RendererAPI");
        return nullptr;
    }
}
