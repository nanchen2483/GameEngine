#pragma once
#include "Engine/Renderer/Buffer/IVertexBuffer.h"
#include "Engine/Renderer/Vertex/Vertex.h"

namespace Engine
{
	class OpenGLVertexBuffer : public IVertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t numOfVertices);
		OpenGLVertexBuffer(Vertex* vertices, uint32_t numOfVertices);
		OpenGLVertexBuffer(const void* vertices, uint32_t size);
		~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const BufferLayout GetLayout() const override { return m_layout; };
		virtual void SetLayout(const BufferLayout layout) override { m_layout = layout; };
		virtual void SetData(const void* data, uint32_t size) override;
	private:
		uint32_t m_rendererId;
		BufferLayout m_layout;
	};
}
