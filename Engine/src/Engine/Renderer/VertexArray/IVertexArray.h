#pragma once
#include "Engine/Renderer/Buffer/VertexBuffer/IVertexBuffer.h"
#include "Engine/Renderer/Buffer/IndexBuffer/IIndexBuffer.h"

namespace Engine
{
	class IVertexArray
	{
	public:
		virtual ~IVertexArray() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ptr<IVertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ptr<IIndexBuffer>& indexBuffer) = 0;
		virtual void SetNumOfPatchVertices(uint32_t numOfVertices) = 0;

		virtual const std::vector<Ptr<IVertexBuffer>>& GetVertexBuffer() const = 0;
		virtual const Ptr<IIndexBuffer>& GetIndexBuffer() const = 0;
		virtual const uint32_t GetNumOfIndices() const = 0;
	};
}
