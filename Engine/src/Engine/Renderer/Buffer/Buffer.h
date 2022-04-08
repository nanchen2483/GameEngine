#pragma once
#include <cstdint>

#include "Engine/Renderer/Vertex/Vertex.h"

namespace Engine
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const BufferLayout GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout layout) = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ptr<VertexBuffer> Create(uint32_t size);
		static Ptr<VertexBuffer> Create(float* vertices, uint32_t numOfVertices);
		static Ptr<VertexBuffer> Create(Vertex* vertices, uint32_t numOfVertices);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetNumOfIndices() const = 0;

		static Ptr<IndexBuffer> Create(uint32_t* indices, uint32_t numOfIndices);
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(const std::vector<const void*>& data) = 0;
		
		static Ptr<UniformBuffer> Create(uint32_t index, const BufferLayout layout);

	};
}
