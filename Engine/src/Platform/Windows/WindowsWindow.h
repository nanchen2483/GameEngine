#pragma once
#include "Engine/Core/Window/IWindow.h"
#include "Engine/Renderer/Graphics/IGraphicsContext.h"
#include "Engine/Renderer/Graphics/IGraphicsLibrary.h"

namespace Engine
{
	class WindowsWindow : public IWindow
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
		inline virtual void* GetNativeWindow() const { return IGraphicsLibrary::GetInstance().GetWindow(); }
		virtual void ShutDown() override;
	private:
		Uniq<IGraphicsContext> m_context;
		WindowUserData m_userData;
	};
}

