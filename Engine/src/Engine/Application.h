#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/LayerStack.h"

class WindowCloseEvent;

namespace Engine
{

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_running = true;
		LayerStack m_layerStack;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

