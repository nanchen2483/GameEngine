#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/Event/ApplicationEvent.h"

namespace Engine
{

	class ENGINE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_window;
		bool m_running = true;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

