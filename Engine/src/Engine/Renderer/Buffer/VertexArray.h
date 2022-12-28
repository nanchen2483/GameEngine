#pragma once
#include "Buffer.h"

namespace Engine
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ptr<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ptr<IndexBuffer>& indexBuffer) = 0;
		virtual void SetNumOfPatchVertices(uint32_t numOfVertices) = 0;

		virtual const std::vector<Ptr<VertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const Ptr<IndexBuffer>& GetIndexBuffer() const = 0;
		virtual const uint32_t GetNumOfIndices() const = 0;

		static Uniq<VertexArray> Create();
	};
}
