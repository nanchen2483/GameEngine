#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Buffer/IBuffer.h"
#include "Engine/Renderer/Buffer/BufferLayout.h"

namespace Engine
{
	class ENGINE_API UniformBuffer : public IBuffer
	{
	public:
		virtual void SetData(const std::vector<const void*>& data) = 0;

		static Ptr<UniformBuffer> Create(uint32_t index, const BufferLayout layout);
	};
}
