#pragma once
#include "Engine/Core/Window/Window.h"
#include "Engine/Renderer/Graphics/GraphicsContext.h"
#include "Engine/Renderer/Graphics/GraphicsLibrary.h"

namespace Engine
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;
		inline uint32_t GetWidth() const override { return m_userData.width; }
		inline uint32_t GetHeight() const override { return m_userData.height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_userData.EventCallback = callback; }
		void SetVSync(bool enable) override;
		bool IsVSync() const override { return m_userData.vSync; }
		inline virtual void* GetNativeWindow() const { return m_library->GetWindow();  }
		virtual void ShutDown() override;
	private:
		Uniq<GraphicsContext> m_context;
		Uniq<GraphicsLibrary> m_library;
		WindowUserData m_userData;
	};
}

