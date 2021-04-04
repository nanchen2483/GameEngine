#pragma once

#include "Engine/Core/Window.h"
#include "Engine/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_data.width; }
		inline unsigned int GetHeight() const override { return m_data.height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window;  }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_window;
		GraphicsContext* m_context;

		struct WindowData
		{
			std::string title;
			unsigned int width = 0, height = 0;
			bool vSync = false;

			EventCallbackFn EventCallback;
		};

		WindowData m_data;
	};
}

