#pragma once
#include "Engine/Renderer/Buffer/IBuffer.h"

namespace Engine
{
	class IUniformBuffer : public IBuffer
	{
	public:
		virtual void SetData(const std::vector<const void*>& data) = 0;
	};
}