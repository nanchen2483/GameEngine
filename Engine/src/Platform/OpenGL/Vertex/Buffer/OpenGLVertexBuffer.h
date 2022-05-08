#pragma once

#include "Engine/Renderer/Vertex/Buffer/Buffer.h"

namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t numOfVertices);
		OpenGLVertexBuffer(Vertex* vertices, uint32_t numOfVertices);
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