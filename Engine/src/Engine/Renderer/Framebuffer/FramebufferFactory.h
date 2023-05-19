#pragma once
#include "IFramebuffer.h"
#include "Engine/Core/Base.h"

namespace Engine
{
	class ENGINE_API FramebufferFactory
	{
	public:
		static Ptr<IFramebuffer> Create(const FramebufferSpecification spec);
	};
}
