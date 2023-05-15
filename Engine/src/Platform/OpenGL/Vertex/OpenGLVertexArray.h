#pragma once
#include "Engine/Renderer/Buffer/IVertexArray.h"

namespace Engine
{
	class OpenGLVertexArray : public IVertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void AddVertexBuffer(const Ptr<IVertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ptr<IIndexBuffer>& indexBuffer) override;
		virtual void SetNumOfPatchVertices(uint32_t numOfVertices) override;

		virtual const std::vector<Ptr<IVertexBuffer>>& GetVertexBuffer() const override { return m_vertexBuffers; };
		virtual const Ptr<IIndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };
		virtual const uint32_t GetNumOfIndices() const override;
	private:
		uint32_t m_rendererId;
		std::vector<Ptr<IVertexBuffer>> m_vertexBuffers;
		Ptr<IIndexBuffer> m_indexBuffer;
	};
}
