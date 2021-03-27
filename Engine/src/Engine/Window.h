#pragma once

#include "enginepch.h"
#include "Engine/Core.h"
#include "Engine/Events/Event.h"

namespace Engine
{
	struct WindowProps
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& title = "My Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			: title(title), width(width), height(height) {}
	};

	class ENGINE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}