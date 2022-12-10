#include "enginepch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "System/System.h"
#include "Window/Input.h"

#include "Engine/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Engine
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	Application::Application(const std::string& name)
	{
		ENGINE_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = Window::Create(WindowProps(name));
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_layerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (std::vector<Engine::Layer*>::iterator it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			if (e.handled)
			{
				break;
			}
			(*--it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		while (m_running)
		{
			System::OnUpdate();

			if (!m_minimized)
			{
				for (Layer* layer : m_layerStack)
				{
					layer->OnUpdate();
				}
			}

			m_imGuiLayer->Begin();
			for (Layer* layer : m_layerStack)
			{
				layer->OnImGuiRender();
			}
			m_imGuiLayer->End();

			m_window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_running = false;
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		m_minimized = false;
		return true;
	}
}
