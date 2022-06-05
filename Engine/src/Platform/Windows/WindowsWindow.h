#pragma once
#include "Engine/Core/Window/Window.h"
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

		inline uint32_t GetWidth() const override { return m_data.width; }
		inline uint32_t GetHeight() const override { return m_data.height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return m_window;  }

	private:
		virtual void Init(const WindowProps& props);
		virtual void ShutDown();

		GLFWwindow* m_window;
		Uniq<GraphicsContext> m_context;

		struct WindowData
		{
			std::string title;
			uint32_t width = 0, height = 0;
			bool vSync = false;

			EventCallbackFn EventCallback;
		} m_data;
	};
}

