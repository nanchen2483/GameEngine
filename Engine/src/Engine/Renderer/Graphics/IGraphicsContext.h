#pragma once

namespace Engine
{
	class IGraphicsContext
	{
	public:
		virtual ~IGraphicsContext() = default;
		virtual void SwapBuffers() = 0;
	};
}
