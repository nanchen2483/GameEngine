#pragma once
#include "IBuffer.h"

namespace Engine
{
	class IIndexBuffer : public IBuffer
	{
	public:
		virtual uint32_t GetNumOfIndices() const = 0;
	};
}
