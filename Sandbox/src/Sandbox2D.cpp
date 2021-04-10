
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

	static bool dockSpaceOpen = false;
	if (dockSpaceOpen)
	{
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) { Engine::Application::Get().Close(); }
				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
	else
	{
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


}

void Sandbox2D::OnEvent(Engine::Event& event)
{
	m_cameraController.OnEvent(event);
}
