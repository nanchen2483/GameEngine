
#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("2D"), m_cameraController(1280.0f / 720.0f), m_color(1.0f)
{
}

void Sandbox2D::OnAttach()
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

	auto colorShader = m_shaderLibrary.Load("Color", "asserts/shaders/Color.glsl");
	m_texture2D = Engine::Texture2D::Create("asserts/textures/blocks.png");
	std::dynamic_pointer_cast<Engine::OpenGLShader>(colorShader)->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(colorShader)->UploadUniformInt("uTexture", 0);
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Engine::TimeStep timeStep)
{
	m_cameraController.OnUpdate(timeStep);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RendererCommand::Clear();

	Engine::Renderer::BeginScene(m_cameraController.GetCamera());

	auto colorShader = m_shaderLibrary.Get("Color");
	m_texture2D->Bind();
	std::dynamic_pointer_cast<Engine::OpenGLShader>(colorShader)->UploadUniformFloat3("uColor", m_color);
	Engine::Renderer::Submit(colorShader, m_vertexArray);

	Engine::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_cameraController.OnEvent(event);
}
