#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Vertex/Vertex.h"
#include "Engine/Renderer/Buffer/IIndexBuffer.h"
#include "Engine/Renderer/Buffer/IUniformBuffer.h"
#include "Engine/Renderer/Buffer/IVertexBuffer.h"

namespace Engine
{
	class ENGINE_API BufferFactory
	{
	public:
		// Vertex Buffer
		static Ptr<IVertexBuffer> CreateVertexBuffer(uint32_t size);
		static Ptr<IVertexBuffer> CreateVertexBuffer(float* vertices, uint32_t numOfVertices);
		static Ptr<IVertexBuffer> CreateVertexBuffer(Vertex* vertices, uint32_t numOfVertices);
		static Ptr<IVertexBuffer> CreateVertexBuffer(const void* vertices, uint32_t size);

		// Index Buffer
		static Ptr<IIndexBuffer> CreateIndexBuffer(uint8_t* indices, uint32_t numOfIndices);
		static Ptr<IIndexBuffer> CreateIndexBuffer(uint16_t* indices, uint32_t numOfIndices);
		static Ptr<IIndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t numOfIndices);

		// Uniform Buffer
		static Ptr<IUniformBuffer> CreateUniformBuffer(uint32_t index, const BufferLayout layout, uint32_t numOfUniforms = 1);
	};
}
