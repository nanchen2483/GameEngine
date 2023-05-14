#pragma once

namespace Engine
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Uniq<GraphicsContext> Create(void *window);
	};
}
