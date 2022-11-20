#pragma once
#include "Base.h"
#include "Layer/LayerStack.h"
#include "Window/Window.h"

#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Camera/OrthographicCamera.h"

class WindowCloseEvent;

namespace Engine
{
	class ENGINE_API Application
	{
	public:
		Application(const std::string& name = "");
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		inline Window& GetWindow() { return *m_window; }
		inline ImGuiLayer* GetImGuiLayer() { return m_imGuiLayer; }
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

