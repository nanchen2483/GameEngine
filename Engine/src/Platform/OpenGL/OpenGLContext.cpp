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

		glfwMakeContextCurrent(m_windowHandler);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENGINE_CORE_ASSERT(status, "Failed to initialize GLAD");

		ENGINE_CORE_INFO("OpenGL Version: {0} ({1})", (const char*)glGetString(GL_VERSION), (const char*)glGetString(GL_RENDERER));
		ENGINE_CORE_INFO("GLSL Version: {0}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

#ifdef ENGINE_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
		ENGINE_CORE_ASSERT(versionMajor > m_minimumMajorVersion || (versionMajor == m_minimumMajorVersion && versionMinor >= m_minimumMinorVersion), "Engine requires at least OpenGL {0}.{1}", m_minimumMajorVersion, m_minimumMinorVersion);
#endif // ENGINE_ENABLE_ASSERTS
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandler);
	}
}
