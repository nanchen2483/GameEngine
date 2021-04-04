
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
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Engine::TimeStep timeStep)
{
	m_cameraController.OnUpdate(timeStep);

	Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RendererCommand::Clear();

	Engine::Renderer2D::BeginScene(m_cameraController.GetCamera());
	Engine::Renderer2D::DrawQuad(glm::vec3(0.5f), glm::vec2(1.0f), m_color);

	Engine::Renderer2D::EndScene();
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
