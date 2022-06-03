#pragma once
#include "Engine/Renderer/Vertex/VertexArray.h"

#include <glad/glad.h>

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ptr<IndexBuffer>& indexBuffer) override;
		virtual void SetNumOfPatchVertices(uint32_t numOfVertices) override;

		virtual const std::vector<Ptr<VertexBuffer>>& GetVertexBuffer() const override { return m_vertexBuffers; };
		virtual const Ptr<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };
		virtual const uint32_t GetNumOfIndices() const override;
	private:
		uint32_t m_rendererId;
		std::vector<Ptr<VertexBuffer>> m_vertexBuffers;
		Ptr<IndexBuffer> m_indexBuffer;
	};
}
