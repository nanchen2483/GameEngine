#include "enginepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandler)
		: m_windowHandler(windowHandler)
	{
		ENGINE_ASSERT(m_windowHandler, "Window handler is null");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_windowHandler);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_CORE_ASSERT(status, "Failed to initialize GLAD");

		ENGINE_CORE_INFO("OpenGL info:");
		ENGINE_CORE_INFO("    Vendor: {0}", glGetString(GL_VENDOR));
		ENGINE_CORE_INFO("    Render: {0}", glGetString(GL_RENDERER));
		ENGINE_CORE_INFO("    Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandler);
	}
}