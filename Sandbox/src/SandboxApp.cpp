#pragma once

#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_cameraPosition(0.0f)
	{
		m_vertexArray.reset(Engine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create(vertices, sizeof(vertices)));
		vertexBuffer->SetLayout(
			{
				{ Engine::ShaderDataType::Float3, "aPosition" },
				{ Engine::ShaderDataType::Float4, "aColor" }
			});
		m_vertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_vertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;
			
			uniform mat4 u_viewProjection;

			out vec3 vPosition;
			out vec4 vColor;
			
			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = u_viewProjection * vec4(aPosition, 1.0);
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

		m_shader.reset(new Engine::Shader(vertexSrc, fragmentSrc));
	}

	void OnUpdate() override
	{
		if (Engine::Input::IsKeyPressed(ENGINE_KEY_LEFT))
		{
			m_cameraPosition.x -= m_cameraMoveSpeed;
		} 
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_RIGHT))
		{
			m_cameraPosition.x += m_cameraMoveSpeed;
		}

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_UP))
		{
			m_cameraPosition.y += m_cameraMoveSpeed;
		}
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_DOWN))
		{
			m_cameraPosition.y -= m_cameraMoveSpeed;
		}

		if (Engine::Input::IsKeyPressed(ENGINE_KEY_A))
		{
			m_cameraRotation += m_cameraRotationSpeed;
		}
		else if (Engine::Input::IsKeyPressed(ENGINE_KEY_D))
		{
			m_cameraRotation -= m_cameraRotationSpeed;
		}

		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RendererCommand::Clear();

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);
		Engine::Renderer::BeginScene(m_camera);
		{
			Engine::Renderer::Submit(m_shader, m_vertexArray);
		}
		Engine::Renderer::EndScene();

	}

	virtual void OnImGuiRender() override
	{
	}

	void OnEvent(Engine::Event& event) override
	{
	}

private:
	std::shared_ptr<Engine::Shader> m_shader;
	std::shared_ptr<Engine::VertexArray> m_vertexArray;

	Engine::OrthographicCamera m_camera;
	glm::vec3 m_cameraPosition;
	float m_cameraRotation = 0.0f;
	float m_cameraMoveSpeed = 0.01f;
	float m_cameraRotationSpeed = 1.0f;
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