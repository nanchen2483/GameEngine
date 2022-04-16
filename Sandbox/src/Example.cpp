#include "Example.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Engine/Renderer/Renderer.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

ExampleLayer::ExampleLayer()
	: Layer("Example"), m_cameraController(1280.0f / 720.0f, true), m_color(glm::vec3(1.0f, 0.0f, 0.0f))
{
}

void ExampleLayer::OnAttach()
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
		{ Engine::ShaderDataType::Float3 },
		{ Engine::ShaderDataType::Float2 },
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

	Engine::Ptr<Engine::Shader> reatangleshader = m_shaderLibrary.Load("Rectangle", vertexSrc, fragmentSrc);
	reatangleshader->Bind();
	reatangleshader->SetInt("uTexture", 0);

	Engine::Ptr<Engine::Shader> textureShader = m_shaderLibrary.Load("Texture", "asserts/shaders/Texture.glsl");
	m_texture2D = Engine::Texture2D::Create("asserts/textures/blocks.png");
	textureShader->Bind();
	textureShader->SetInt("uTexture", 0);
}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(Engine::TimeStep timeStep)
{
	m_cameraController.OnUpdate(timeStep);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RendererCommand::Clear();

	Engine::Renderer::BeginScene(m_cameraController.GetCamera());

	Engine::Ptr<Engine::Shader> reatangleShader = m_shaderLibrary.Get("Rectangle");
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
				reatangleShader->SetFloat3("uColor", m_color);
			}
			else
			{
				reatangleShader->SetFloat3("uColor", glm::vec3(0.0f, 0.0f, 1.0f));
			}
			Engine::Renderer::Submit(reatangleShader, m_vertexArray, transform);
		}
	}

	Engine::Ptr<Engine::Shader> textureShader = m_shaderLibrary.Get("Texture");
	textureShader->Bind();
	m_texture2D->Bind();
	Engine::Renderer::Submit(textureShader, m_vertexArray);

	Engine::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
	ImGui::End();
}

void ExampleLayer::OnEvent(Engine::Event& event)
{
	m_cameraController.OnEvent(event);
}
