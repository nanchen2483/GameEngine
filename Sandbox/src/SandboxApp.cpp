#pragma once

#include <Engine.h>

#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_cameraController(1280.0f / 720.0f, true)
	{
		m_vertexArray = Engine::VertexArray::Create();

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Engine::Ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer = Engine::VertexBuffer::Create(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ Engine::ShaderDataType::Float3, "aPosition" },
			{ Engine::ShaderDataType::Float2, "aTexCoord" },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine::Ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer = Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec2 aTexCoord;
			
			uniform mat4 uViewProjection;
			uniform mat4 uModel;

			out vec3 vPosition;
			out vec2 vTexCoord;
			
			void main()
			{
				vPosition = aPosition;
				vTexCoord = aTexCoord;
				gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			
			in vec3 vPosition;
			in vec2 vTexCoord;
			
			uniform vec3 uColor;
			uniform sampler2D uTexture;
			
			void main()
			{
				color = vec4(uColor, 1.0);
			}
		)";

		auto reatangleshader = m_shaderLibrary.Load("Rectangle", vertexSrc, fragmentSrc);
		std::dynamic_pointer_cast<Engine::OpenGLShader>(reatangleshader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(reatangleshader)->UploadUniformInt("uTexture", 0);

		auto textureShader = m_shaderLibrary.Load("asserts/shaders/Texture.glsl");
		m_texture2D = Engine::Texture2D::Create("asserts/textures/blocks.png");
		std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(textureShader)->UploadUniformInt("uTexture", 0);
	}

	void OnUpdate(Engine::TimeStep timeStep) override
	{
		m_cameraController.OnUpdate(timeStep);

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RendererCommand::Clear();

		Engine::Renderer::BeginScene(m_cameraController.GetCamera());
		
		auto reatangleShader = m_shaderLibrary.Get("Rectangle");
		reatangleShader->Bind();
		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x + y) % 2)
				{
					std::dynamic_pointer_cast<Engine::OpenGLShader>(reatangleShader)->UploadUniformFloat3("uColor", m_color);
				}
				else
				{
					std::dynamic_pointer_cast<Engine::OpenGLShader>(reatangleShader)->UploadUniformFloat3("uColor", glm::vec3(0.0f, 0.0f, 1.0f));
				}
				Engine::Renderer::Submit(reatangleShader, m_vertexArray, transform);
			}
		}

		auto textureShader = m_shaderLibrary.Get("Texture");
		textureShader->Bind();
		m_texture2D->Bind();
		Engine::Renderer::Submit(textureShader, m_vertexArray);

		Engine::Renderer::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
		ImGui::End();
	}

	void OnEvent(Engine::Event& event) override
	{
		m_cameraController.OnEvent(event);
	}

private:
	Engine::ShaderLibrary m_shaderLibrary;
	Engine::Ptr<Engine::VertexArray> m_vertexArray;
	Engine::Ptr<Engine::Texture2D> m_texture2D;

	Engine::OrthographicCameraController m_cameraController;
	glm::vec3 m_color = glm::vec3(1.0f, 0.0f, 0.0f);
};


class Sandbox : public Engine::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

Engine::Application* Engine::CreateApplication()
{
	return new Sandbox();
}