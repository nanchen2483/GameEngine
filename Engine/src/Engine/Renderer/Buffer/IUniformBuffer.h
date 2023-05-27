#pragma once
#include "IBuffer.h"

namespace Engine
{
	class IUniformBuffer : public IBuffer
	{
	public:
		virtual void SetData(const std::vector<const void*>& data) = 0;
		virtual void SetData(const std::vector<std::vector<const void*>>& data) = 0;
	};
}
