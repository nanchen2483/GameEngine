#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Renderer/Graphics/GraphicsContext/IGraphicsContext.h"
#include "WindowProperties.h"
#include "WindowUserData.h"

namespace Engine
{
	class ENGINE_API Window
	{
	public:
		Window(const WindowProperties& properties);
		~Window();
		
		void OnUpdate();
		uint32_t GetWidth() const { return m_userData.width; }
		uint32_t GetHeight() const { return m_userData.height; }
		bool IsVSync() const { return m_userData.vSync; }
		void SetVSync(bool enable);
		void SetEventCallback(const EventCallbackFn& callback);
		void* GetNativeWindow() const;
		void ShutDown();
	private:
		Uniq<IGraphicsContext> m_context;
		WindowUserData m_userData;
	};
}
