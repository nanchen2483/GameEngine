#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Events/Event.h"

namespace Engine
{
	struct WindowProps
	{
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& title = "My Engine",
					uint32_t width = 1280,
					uint32_t height = 720)
			: title(title), width(width), height(height) {}
	};

	class ENGINE_API Window
	{
	public:
		virtual ~Window() {}
		
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		using EventCallbackFn = std::function<void(Event&)>;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Uniq<Window> Create(const WindowProps& props = WindowProps());
	};
}
