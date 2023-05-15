#pragma once
#include "Engine/Core/Base.h"

namespace Engine
{
	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Uniq<IGraphicsContext> Create(void *window);
	};
}
