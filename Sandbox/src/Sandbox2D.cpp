
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
	ENGINE_PROFILE_FUNCTION();

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

	Engine::Renderer2D::ResetStates();
	{
		ENGINE_PROFILE_SCOPE("Renderer Prep");
		Engine::RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		Engine::RendererCommand::Clear();
	}

	{
		ENGINE_PROFILE_SCOPE("Renderer Draw");
		Engine::Renderer2D::BeginScene(m_cameraController.GetCamera());
		Engine::Renderer2D::DrawQuad(glm::vec3(1.0f), glm::vec2(1.0f), m_color);
		Engine::Renderer2D::DrawQuad(glm::vec3(0.0f), glm::vec2(1.0f), m_color);
		Engine::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0, -0.1f), glm::vec2(10.0f), m_texture2D);

		Engine::Renderer2D::EndScene();

		Engine::Renderer2D::BeginScene(m_cameraController.GetCamera());
		for (float y = -4.5f; y < 5.0f; y += 0.5f)
		{
			for (float x = -4.5f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = glm::vec4((x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f);
				Engine::Renderer2D::DrawQuad(glm::vec3(x, y, 0.0f), glm::vec2(0.45f, 0.45f), color);
			}
		}

		Engine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ENGINE_PROFILE_FUNCTION();
	
	ImGui::Begin("Settings");

	auto stats = Engine::Renderer2D::GetState();
	ImGui::Text("Renderer stats");
	ImGui::Text("Draw Calls: %d", stats.drawCalls);
	ImGui::Text("Quads: %d", stats.quadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_color));
	ImGui::Image((void*)m_texture2D->GetRendererId(), ImVec2(256.0f, 256.0f));
	ImGui::End();

}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_cameraController.OnEvent(event);
}
