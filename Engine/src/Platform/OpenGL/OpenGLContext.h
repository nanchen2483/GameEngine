#pragma once
#include "Engine/Renderer/Graphics/IGraphicsContext.h"

struct GLFWwindow;

namespace Engine
{
	class OpenGLContext : public IGraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandler);
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_windowHandler;
	};
}

