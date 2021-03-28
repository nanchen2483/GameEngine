#pragma once

#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Engine
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandler);

		virtual void Init() override;
		virtual void SwapBuffers() override;

	private:
		GLFWwindow* m_windowHandler;
	};
}

