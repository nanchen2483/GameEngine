#include "enginepch.h"
#include "WindowsWindow.h"

#include "Engine/Core/Enum/KeyMouseButtonActions.h" 
#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"

namespace Engine
{
	WindowsWindow::WindowsWindow(const WindowProperties& properties)
	{
		ENGINE_CORE_INFO("Creating window: {0} ({1}x{2})", properties.title, properties.width, properties.height);

		m_userData.title = properties.title;
		m_userData.width = properties.width;
		m_userData.height = properties.height;

		m_library = GraphicsLibrary::Create();
		m_library->CreateNewWindow(properties.title, properties.width, properties.height);
		m_library->SetWindowUserDataPointer(&m_userData);
		m_context = m_library->GetContext();
		SetVSync(true);

		// Set GLFW callbacks
		m_library->SetWindowsSizeCallback([](void* window, int32_t width, int32_t height) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);
			data->width = width;
			data->height = height;

			WindowResizeEvent event(width, height);
			data->EventCallback(event);
		});

		m_library->SetWindowCloseCallback([](void* window) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);

			WindowCloseEvent event;
			data->EventCallback(event);
		});

		m_library->SetKeyCallback([](void* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);

			switch (action)
			{
			case KeyMouseButtonActions::Press:
			{
				KeyPressedEvent event(key, 0);
				data->EventCallback(event);
				break;
			}
			case KeyMouseButtonActions::Release:
			{
				KeyReleasedEvent event(key);
				data->EventCallback(event);
				break;
			}
			case KeyMouseButtonActions::Repeat:
			{
				KeyPressedEvent event(key, 1);
				data->EventCallback(event);
				break;
			}
			default:
				break;
			}
		});

		m_library->SetCharCallback([](void* window, uint32_t keycode) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);

			KeyTypedEvent event(keycode);
			data->EventCallback(event);
		});

		m_library->SetMouseButtonCallback([](void* window, int32_t button, int32_t action, int32_t mods) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);

			switch (action)
			{
			case KeyMouseButtonActions::Press:
			{
				MouseButtonPressedEvent event(button);
				data->EventCallback(event);
				break;
			}
			case KeyMouseButtonActions::Release:
			{
				MouseButtonReleasedEvent event(button);
				data->EventCallback(event);
				break;
			}
			default:
				break;
			}
		});

		m_library->SetScrollCallback([](void* window, double xOffset, double yOffset) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);
			
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data->EventCallback(event);
		});

		m_library->SetCursorPosCallback([](void* window, double xPos, double yPos) {
			WindowUserData* data = GraphicsLibrary::GetWindowUserDataPointerStatic(window);
			
			MouseMovedEvent event((float)xPos, (float)yPos);
			data->EventCallback(event);
		});
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::OnUpdate()
	{
		m_library->PollEvents();
		m_context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enable)
	{
		m_library->SetVSync(enable);
		m_userData.vSync = enable;
	}

	void WindowsWindow::ShutDown()
	{
		m_library->DestroyWindow();
	}
}
