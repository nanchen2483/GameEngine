#pragma once
#include "BufferLayout.h"
#include "Engine/Renderer/Model/Vertex.h"

namespace Engine
{
	class IVertexBuffer
	{
	public:
		virtual ~IVertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual const BufferLayout GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout layout) = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ptr<IVertexBuffer> Create(uint32_t size);
		static Ptr<IVertexBuffer> Create(float* vertices, uint32_t numOfVertices);
		static Ptr<IVertexBuffer> Create(Vertex* vertices, uint32_t numOfVertices);
		static Ptr<IVertexBuffer> Create(const void* vertices, uint32_t size);
	};

	class IIndexBuffer
	{
	public:
		virtual ~IIndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetNumOfIndices() const = 0;

		static Ptr<IIndexBuffer> Create(uint8_t* indices, uint32_t numOfIndices);
		static Ptr<IIndexBuffer> Create(uint16_t* indices, uint32_t numOfIndices);
		static Ptr<IIndexBuffer> Create(uint32_t* indices, uint32_t numOfIndices);
	};

	class ENGINE_API IUniformBuffer
	{
	public:
		virtual ~IUniformBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(const std::vector<const void*>& data) = 0;
		
		static Ptr<IUniformBuffer> Create(uint32_t index, const BufferLayout layout);
	};
}
