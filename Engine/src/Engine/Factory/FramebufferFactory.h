#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Framebuffer/IFramebuffer.h"

namespace Engine
{
	class ENGINE_API FramebufferFactory
	{
	public:
		static Ptr<IFramebuffer> Create(const FramebufferSpecification spec);
	};
}
