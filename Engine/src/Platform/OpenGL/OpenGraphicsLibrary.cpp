#include "enginepch.h"
#include "OpenGraphicsLibrary.h"

#include "Platform/Util/OpenGLUtil.h"
#include <GLFW/glfw3.h>

namespace Engine
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ENGINE_CORE_ERROR("GLFW Error {0}: {1}", error, description);
	}

	OpenGraphicsLibrary::OpenGraphicsLibrary()
		: m_window(nullptr), m_context(nullptr)
	{
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			ENGINE_CORE_ASSERT(success, "Could not initialize GLFW");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}
	}

	OpenGraphicsLibrary::~OpenGraphicsLibrary()
	{
	}

	void OpenGraphicsLibrary::CreateNewWindow(std::string title, uint32_t width, uint32_t height)
	{
		m_window = glfwCreateWindow((int)width, (int)height, title.c_str(), nullptr, nullptr);
		m_context = IGraphicsContext::Create((void*)m_window);
		m_context->Init();
	}

	void OpenGraphicsLibrary::DestroyWindow()
	{
		glfwDestroyWindow(m_window);
	}

	void OpenGraphicsLibrary::SetWindowUserDataPointer(WindowUserData* userData)
	{
		glfwSetWindowUserPointer(m_window, static_cast<void*>(userData));
	}

	WindowUserData* OpenGraphicsLibrary::GetWindowUserDataPointer() const
	{
		return GetWindowUserDataPointerStatic(m_window);
	}

	void OpenGraphicsLibrary::SetVSync(bool enable)
	{
		glfwSwapInterval(enable ? 1 : 0);
	}

	void OpenGraphicsLibrary::PollEvents()
	{
		glfwPollEvents();
	}

	KeyMouseButtonActions OpenGraphicsLibrary::GetKeyState(KeyCode keycode) const
	{
		return static_cast<KeyMouseButtonActions>(glfwGetKey(m_window, keycode));
	}

	KeyMouseButtonActions OpenGraphicsLibrary::GetMouseButtonState(MouseButton button) const
	{
		return static_cast<KeyMouseButtonActions>(glfwGetMouseButton(m_window, button));
	}

	std::pair<float, float> OpenGraphicsLibrary::GetCursorPosition() const
	{
		double xPos, yPos;
		glfwGetCursorPos(m_window, &xPos, &yPos);

		return { xPos, yPos };
	}

	CursorMode OpenGraphicsLibrary::GetCursorMode() const
	{
		return OpenGLUtil::FromGLCursorMode(glfwGetInputMode(m_window, GLFW_CURSOR));
	}

	void OpenGraphicsLibrary::SetCursorMode(CursorMode mode)
	{
		glfwSetInputMode(m_window, GLFW_CURSOR, OpenGLUtil::ToGL(mode));
	}

	void OpenGraphicsLibrary::SetWindowsSizeCallback(void (*callback)(void* window, int32_t width, int32_t height))
	{
		glfwSetWindowSizeCallback(m_window, (GLFWwindowsizefun)callback);
	}

	void OpenGraphicsLibrary::SetWindowCloseCallback(void(*callback)(void* window))
	{
		glfwSetWindowCloseCallback(m_window, (GLFWwindowclosefun)callback);
	}

	void OpenGraphicsLibrary::SetKeyCallback(void(*callback)(void* window, int32_t key, int32_t scancode, int32_t action, int32_t mods))
	{
		glfwSetKeyCallback(m_window, (GLFWkeyfun)callback);
	}

	void OpenGraphicsLibrary::SetCharCallback(void(*callback)(void* window, uint32_t keycode))
	{
		glfwSetCharCallback(m_window, (GLFWcharfun)callback);
	}

	void OpenGraphicsLibrary::SetMouseButtonCallback(void(*callback)(void* window, int32_t button, int32_t action, int32_t mods))
	{
		glfwSetMouseButtonCallback(m_window, (GLFWmousebuttonfun)callback);
	}

	void OpenGraphicsLibrary::SetScrollCallback(void(*callabck)(void* window, double xOffset, double yOffset))
	{
		glfwSetScrollCallback(m_window, (GLFWscrollfun)callabck);
	}

	void OpenGraphicsLibrary::SetCursorPosCallback(void(*callback)(void* window, double xPos, double yPos))
	{
		glfwSetCursorPosCallback(m_window, (GLFWcursorposfun)callback);
	}

	WindowUserData* OpenGraphicsLibrary::GetWindowUserDataPointerStatic(void* window)
	{
		return static_cast<WindowUserData*>(glfwGetWindowUserPointer(static_cast<GLFWwindow*>(window)));
	}
}
