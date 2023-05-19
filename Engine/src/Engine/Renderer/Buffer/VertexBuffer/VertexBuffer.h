#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Buffer/IBuffer.h"
#include "Engine/Renderer/Buffer/BufferLayout.h"
#include "Engine/Renderer/Vertex/Vertex.h"

namespace Engine
{
	class VertexBuffer : public IBuffer
	{
	public:
		virtual const BufferLayout GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout layout) = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ptr<VertexBuffer> Create(uint32_t size);
		static Ptr<VertexBuffer> Create(float* vertices, uint32_t numOfVertices);
		static Ptr<VertexBuffer> Create(Vertex* vertices, uint32_t numOfVertices);
		static Ptr<VertexBuffer> Create(const void* vertices, uint32_t size);
	};
}
