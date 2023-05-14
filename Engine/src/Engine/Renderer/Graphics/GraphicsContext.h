#pragma once

struct IWindow;

namespace Engine
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static Uniq<GraphicsContext> Create(IWindow *window);
	};
}
