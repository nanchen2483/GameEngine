
#include "EditorLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_cameraController(1280.0f / 720.0f), m_color(0.2, 0.2, 1.0f, 1.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		ENGINE_PROFILE_FUNCTION();

		m_texture2D = Texture2D::Create("asserts/textures/blocks.png");
	
		FramebufferSpecification fbSpec;
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::Create(fbSpec);


		// Entity
		m_activeScene = CreatePtr<Scene>();
		m_squareEntity = m_activeScene->CreateEntity("Lightblue square");
		m_squareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.6f, 0.8f, 1.0f, 1.0f));

		m_cameraEntity = m_activeScene->CreateEntity("Camera entity");
		m_cameraEntity.AddComponent<CameraComponent>();

		m_secondCameraEntity = m_activeScene->CreateEntity("Second camera entity");
		m_secondCameraEntity.AddComponent<CameraComponent>(false);

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnCreate() override
			{
			}

			virtual void OnDestroy() override
			{

			}

			virtual void OnUpdate(TimeStep ts) override
			{
				auto& transform = GetComponent<TransformComponent>().transform;
				float speed = 5.0f;

				if (Input::IsKeyPressed(ENGINE_KEY_A))
				{
					transform[3][0] -= speed * ts;
				}
				if (Input::IsKeyPressed(ENGINE_KEY_D))
				{
					transform[3][0] += speed * ts;
				}
				if (Input::IsKeyPressed(ENGINE_KEY_W))
				{
					transform[3][1] -= speed * ts;
				}
				if (Input::IsKeyPressed(ENGINE_KEY_S))
				{
					transform[3][1] += speed * ts;
				}
			}
		};

		m_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		ENGINE_PROFILE_FUNCTION();

		if (FramebufferSpecification spec = m_framebuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
		{
			m_framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_cameraController.OnResize(m_viewportSize.x, m_viewportSize.y);
			m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if (m_viewportFocused)
		{
			ENGINE_PROFILE_SCOPE("Camera OnUpdate");
			m_cameraController.OnUpdate(timeStep);
		}

		Renderer2D::ResetStates();
		m_framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();

		{
			//ENGINE_PROFILE_SCOPE("Renderer Draw");
			//Renderer2D::BeginScene(m_cameraController.GetCamera());
			//{
			//	m_activeScene->OnUpdate(timeStep);
			//	Renderer2D::DrawQuad(glm::vec3(1.0f), glm::vec2(1.0f), m_color);
			//	Renderer2D::DrawQuad(glm::vec3(0.0f), glm::vec2(1.0f), m_color);
			//	Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0, -0.1f), glm::vec2(10.0f), m_texture2D);
			//}
			//Renderer2D::EndScene();

			//Renderer2D::BeginScene(m_cameraController.GetCamera());
			//for (float y = -4.5f; y < 5.0f; y += 0.5f)
			//{
			//	for (float x = -4.5f; x < 5.0f; x += 0.5f)
			//	{
			//		glm::vec4 color = glm::vec4((x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f);
			//		Renderer2D::DrawQuad(glm::vec3(x, y, 0.0f), glm::vec2(0.45f, 0.45f), color);
			//	}
			//}
			//Renderer2D::EndScene();
		}

		m_activeScene->OnUpdate(timeStep);

		m_framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ENGINE_PROFILE_FUNCTION();

		static bool dockSpaceOpen = true;
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
				if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
				ImGui::Separator();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		auto stats = Renderer2D::GetState();
		ImGui::Text("Renderer stats");
		ImGui::Text("Draw Calls: %d", stats.drawCalls);
		ImGui::Text("Quads: %d", stats.quadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		if (m_squareEntity)
		{
			ImGui::Separator();
			auto& tag = m_squareEntity.GetComponent<TagComponent>().tag;
			ImGui::Text("%s", tag.c_str());
			auto& squareColor = m_squareEntity.GetComponent<SpriteRendererComponent>().color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}

		ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_cameraEntity.GetComponent<TransformComponent>().transform[3]));
		if (ImGui::Checkbox("Primary Camera", &m_isPrimaryCamera))
		{
			m_cameraEntity.GetComponent<CameraComponent>().primary = m_isPrimaryCamera;
			m_secondCameraEntity.GetComponent<CameraComponent>().primary = !m_isPrimaryCamera;
		}

		{
			auto& camera = m_secondCameraEntity.GetComponent<CameraComponent>().camera;
			float orthoSize = camera.GetOrthographicSize();
			if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
			{
				camera.SetOrthographicSize(orthoSize);
			}
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Renderer");
		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->DisableEvents(m_viewportFocused && m_viewportHovered);
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		m_viewportSize = glm::vec2(viewportSize.x, viewportSize.y);
		ImGui::Image((void*)m_framebuffer->GetColorAttachmentRendererId(), ImVec2(m_viewportSize.x, m_viewportSize.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResize)
		{
			auto e = (WindowResizeEvent*)&event;
			if (e->GetWidth() == 0 || e->GetHeight() == 0)
			{
				m_viewportSize = glm::vec2(0.0f);
			}
		}

		m_cameraController.OnEvent(event);
	}
}
