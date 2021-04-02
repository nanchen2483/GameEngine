#pragma once

#include "Core.h"

#include "Window.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/LayerStack.h"
#include <Engine/Renderer/Shader.h>
#include <Engine/Renderer/Buffer.h>

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

		std::unique_ptr<Window> m_window;
		ImGuiLayer* m_imGuiLayer;
		bool m_running = true;
		LayerStack m_layerStack;

		unsigned int m_vertexArray;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexBuffer> m_vertexBuffer;
		std::unique_ptr<IndexBuffer> m_indexBuffer;
	private:
		static Application* s_instance;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();
}

