#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Core/LayerStack.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexArray.h"
#include "Engine/Renderer/OrthographicCamera.h"

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
		inline Window& GetWindow() { return *m_window; }
		inline static Application& Get() { return *s_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Uniq<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		bool m_minimized = false;
		LayerStack m_layerStack;
		float m_lastFrameTime = 0.0f;
	private:
		static Application* s_instance;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

