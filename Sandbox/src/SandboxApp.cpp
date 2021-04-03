#pragma once

#include <Engine.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_cameraPosition(0.0f), m_cameraRotation(0.0f)
	{
		m_vertexArray.reset(Engine::VertexArray::Create());

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Engine::Ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout({
			{ Engine::ShaderDataType::Float3, "aPosition" },
			{ Engine::ShaderDataType::Float2, "aTexCoord" },
		});
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Engine::Ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
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
				color = texture(uTexture, vTexCoord);
			}
		)";

		m_shader.reset(Engine::Shader::Create(vertexSrc, fragmentSrc));

		m_texture2D = Engine::Texture2D::Create("asserts/textures/blocks.png");

		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader)->Bind();
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader)->UploadUniformInt("uTexture", 0);
	}

	void OnUpdate(Engine::TimeStep timeStep) override
	{
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
		{
			m_cameraPosition.x -= m_cameraMoveSpeed * timeStep;
		} 
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
		{
			m_cameraPosition.x += m_cameraMoveSpeed * timeStep;
		}

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_W))
		{
			m_cameraPosition.y += m_cameraMoveSpeed * timeStep;
		}
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_S))
		{
			m_cameraPosition.y -= m_cameraMoveSpeed * timeStep;
		}

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_Q))
		{
			m_cameraRotation += m_cameraRotationSpeed * timeStep;
		}
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_E))
		{
			m_cameraRotation -= m_cameraRotationSpeed * timeStep;
		}

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RendererCommand::Clear();

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);
		Engine::Renderer::BeginScene(m_camera);

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader)->Bind();
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if ((x + y) % 2)
				{
					std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader)->UploadUniformFloat3("uColor", m_color);
				}
				else
				{
					std::dynamic_pointer_cast<Engine::OpenGLShader>(m_shader)->UploadUniformFloat3("uColor", glm::vec3(0.0f, 0.0f, 1.0f));
				}
				Engine::Renderer::Submit(m_shader, m_vertexArray, transform);
			}
		}

		m_texture2D->Bind();
		Engine::Renderer::Submit(m_shader, m_vertexArray);

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
	}

private:
	Engine::Ptr<Engine::Shader> m_shader;
	Engine::Ptr<Engine::VertexArray> m_vertexArray;
	Engine::Ptr<Engine::Texture2D> m_texture2D;

	Engine::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraMoveSpeed = 1.0f;
	float m_cameraRotation;
	float m_cameraRotationSpeed = 180.0f;

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