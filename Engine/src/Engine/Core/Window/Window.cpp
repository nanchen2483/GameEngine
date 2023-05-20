#include "enginepch.h"
#include "Window.h"

#include "Engine/Core/Enum/KeyMouseButtonActions.h" 
#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"
#include "Engine/Factory/GraphicsFactory.h"

namespace Engine
{
	Window::Window(const WindowProperties& properties)
	{
		ENGINE_CORE_INFO("Creating window: {0} ({1}x{2})", properties.title, properties.width, properties.height);

		m_userData.title = properties.title;
		m_userData.width = properties.width;
		m_userData.height = properties.height;

		GraphicsFactory::GetLibraryInstance().CreateNewWindow(properties.title, properties.width, properties.height);
		GraphicsFactory::GetLibraryInstance().SetWindowUserDataPointer(&m_userData);
		m_context = GraphicsFactory::GetLibraryInstance().GetContext();
		SetVSync(true);

		// Set callbacks
		GraphicsFactory::GetLibraryInstance().SetWindowsSizeCallback(
			[](void* window, int32_t width, int32_t height)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);
				data->width = width;
				data->height = height;

				WindowResizeEvent event(width, height);
				data->EventCallback(event);
			});

		GraphicsFactory::GetLibraryInstance().SetWindowCloseCallback(
			[](void* window)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);

				WindowCloseEvent event;
				data->EventCallback(event);
			});

		GraphicsFactory::GetLibraryInstance().SetKeyCallback(
			[](void* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);

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

		GraphicsFactory::GetLibraryInstance().SetCharCallback(
			[](void* window, uint32_t keycode)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);

				KeyTypedEvent event(keycode);
				data->EventCallback(event);
			});

		GraphicsFactory::GetLibraryInstance().SetMouseButtonCallback(
			[](void* window, int32_t button, int32_t action, int32_t mods)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);

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

		GraphicsFactory::GetLibraryInstance().SetScrollCallback(
			[](void* window, double xOffset, double yOffset)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data->EventCallback(event);
			});

		GraphicsFactory::GetLibraryInstance().SetCursorPosCallback(
			[](void* window, double xPos, double yPos)
			{
				WindowUserData* data = GraphicsFactory::GetWindowUserDataPointerStatic(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data->EventCallback(event);
			});
	}

	Window::~Window()
	{
		ShutDown();
	}

	void Window::OnUpdate()
	{
		GraphicsFactory::GetLibraryInstance().PollEvents();
		m_context->SwapBuffers();
	}

	void Window::SetVSync(bool enable)
	{
		GraphicsFactory::GetLibraryInstance().SetVSync(enable);
		m_userData.vSync = enable;
	}

	void Window::SetEventCallback(const EventCallbackFn& callback)
	{
		m_userData.EventCallback = callback;
	}

	void* Window::GetNativeWindow() const
	{
		return GraphicsFactory::GetLibraryInstance().GetWindow();
	}

	void Window::ShutDown()
	{
		GraphicsFactory::GetLibraryInstance().DestroyWindow();
	}
}
