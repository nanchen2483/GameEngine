#pragma once
#include "Engine/Core/Base.h"
#include "WindowProperties.h"
#include "WindowUserData.h"

namespace Engine
{
	class ENGINE_API Window
	{
	public:
		virtual ~Window() {}
		
		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;
		virtual void ShutDown() = 0;

		static Uniq<Window> Create(const WindowProperties& props = WindowProperties());
	};
}
