
#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sanbox2D"), m_cameraController(1280.0f / 720.0f), m_color(0.2, 0.2, 1.0f, 1.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_texture2D = Engine::Texture2D::Create("asserts/textures/blocks.png");
}

void Sandbox2D::OnDetach()
{

}
void Sandbox2D::OnUpdate(Engine::TimeStep timeStep)
{
	ENGINE_PROFILE_FUNCTION();

	{
		ENGINE_PROFILE_SCOPE("Camera OnUpdate");
		m_cameraController.OnUpdate(timeStep);
	}

	{
		ENGINE_PROFILE_SCOPE("Renderer Prep");
		Engine::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RendererCommand::Clear();
	}

	{
		ENGINE_PROFILE_SCOPE("Renderer Draw");
		Engine::Renderer2D::BeginScene(m_cameraController.GetCamera());
		Engine::Renderer2D::DrawQuad(glm::vec3(0.5f), glm::vec2(0.5f), m_color);
		//Engine::Renderer2D::DrawQuad(glm::vec3(0.5f), glm::vec2(5.0f), m_texture2D);

		Engine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_cameraController.OnEvent(event);
}
