#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Buffer/IBuffer.h"

namespace Engine
{
	class IndexBuffer : public IBuffer
	{
	public:
		virtual uint32_t GetNumOfIndices() const = 0;

		static Ptr<IndexBuffer> Create(uint8_t* indices, uint32_t numOfIndices);
		static Ptr<IndexBuffer> Create(uint16_t* indices, uint32_t numOfIndices);
		static Ptr<IndexBuffer> Create(uint32_t* indices, uint32_t numOfIndices);
	};
}
