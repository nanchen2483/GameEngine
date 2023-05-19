#pragma once
#include "Engine/Renderer/Buffer/IBuffer.h"
#include "Engine/Renderer/Buffer/BufferLayout.h"

namespace Engine
{
	class IVertexBuffer : public IBuffer
	{
	public:
		virtual const BufferLayout GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout layout) = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
	};
}
