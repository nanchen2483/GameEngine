#include "enginepch.h"

#include "Application.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Input.h"

#include <glad/glad.h>

namespace Engine
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:		return GL_FLOAT;
		case Engine::ShaderDataType::Float2:	return GL_FLOAT;
		case Engine::ShaderDataType::Float3:	return GL_FLOAT;
		case Engine::ShaderDataType::Float4:	return GL_FLOAT;
		case Engine::ShaderDataType::Mat3:		return GL_FLOAT;
		case Engine::ShaderDataType::Mat4:		return GL_FLOAT;
		case Engine::ShaderDataType::Int:		return GL_INT;
		case Engine::ShaderDataType::Int2:		return GL_INT;
		case Engine::ShaderDataType::Int3:		return GL_INT;
		case Engine::ShaderDataType::Int4:		return GL_INT;
		case Engine::ShaderDataType::Bool:		return GL_BOOL;
		}

		ENGINE_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	Application::Application()
	{
		ENGINE_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_imGuiLayer = new ImGuiLayer();
		PushOverlay(m_imGuiLayer);

		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "aPosition" },
				{ ShaderDataType::Float4, "aColor" }
			};

			m_vertexBuffer->SetLayout(layout);
		}

		uint32_t index = 0;
		const auto& layout = m_vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void *)element.offset);
			index++;
		}

		uint32_t indices[3] = { 0, 1, 2 };
		m_indexBuffer.reset(IndexBuffer::Create(indices, 3));

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;
			
			out vec3 vPosition;
			out vec4 vColor;
			
			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = vec4(aPosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 vPosition;
			in vec4 vColor;
			
			void main()
			{
				color = vColor;
			}
		)";

		m_shader.reset(new Shader(vertexSrc, fragmentSrc));
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
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.handled)
			{
				break;
			}
		}
	}

	void Application::Run()
	{
		while (m_running)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_shader->Bind();
			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_layerStack)
			{
				layer->OnUpdate();
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

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}