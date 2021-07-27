
#include "EditorLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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
		fbSpec.attachments = FramebufferAttachmentSpecification({ FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH24STENCIL8 });
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::Create(fbSpec);

		// Entity
		m_activeScene = CreatePtr<Scene>();

		m_editorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0
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
				auto& translation = GetComponent<TransformComponent>().translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(ENGINE_KEY_A))
				{
					translation.x -= speed * ts;
				}
				if (Input::IsKeyPressed(ENGINE_KEY_D))
				{
					translation.x += speed * ts;
				}
				if (Input::IsKeyPressed(ENGINE_KEY_W))
				{
					translation.y += speed * ts;
				}
				if (Input::IsKeyPressed(ENGINE_KEY_S))
				{
					translation.y -= speed * ts;
				}
			}
		};

		m_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif // 0

		m_sceneHierachyPanel.SetContext(m_activeScene);
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
			m_editorCamera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if (m_viewportFocused)
		{
			ENGINE_PROFILE_SCOPE("Camera OnUpdate");
			m_cameraController.OnUpdate(timeStep);
		}

		m_editorCamera.OnUpdate(timeStep);

		Renderer2D::ResetStates();
		m_framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();

		// Clear entity id buffer attachment to -1
		m_framebuffer->ClearAttachment(1, -1);

#if 0
		{
			ENGINE_PROFILE_SCOPE("Renderer Draw");
			Renderer2D::BeginScene(m_cameraController.GetCamera());
			{
				m_activeScene->OnUpdateRuntime(timeStep);
				Renderer2D::DrawQuad(glm::vec3(1.0f), glm::vec2(1.0f), m_color);
				Renderer2D::DrawQuad(glm::vec3(0.0f), glm::vec2(1.0f), m_color);
				Renderer2D::DrawQuad(glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0, -0.1f)), m_texture2D);
			}
			Renderer2D::EndScene();

			Renderer2D::BeginScene(m_cameraController.GetCamera());
			for (float y = -4.5f; y < 5.0f; y += 0.5f)
			{
				for (float x = -4.5f; x < 5.0f; x += 0.5f)
				{
					glm::vec4 color = glm::vec4((x + 0.5f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f);
					Renderer2D::DrawQuad(glm::vec3(x, y, 0.0f), glm::vec2(0.45f, 0.45f), color);
				}
			}
			Renderer2D::EndScene();
		}
#endif // 0

		m_activeScene->OnUpdateEditor(timeStep, m_editorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = { m_viewportBounds[1] - m_viewportBounds[0] };
		my = m_viewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;
		
		if (mouseX > 0 && mouseY > 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
		{
			int pixelData = m_framebuffer->ReadPixel(1, mouseX, mouseY);
			if (pixelData == -1)
			{
				m_hoverdEntity = Entity();
			}
			else
			{
				m_hoverdEntity = Entity((entt::entity)pixelData, m_activeScene.get());
			}

			ENGINE_WARN("entityId {0}", pixelData);
		}

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
		{
			window_flags |= ImGuiWindowFlags_NoBackground;
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		}
		
		if (ImGui::Begin("DockSpace", &dockSpaceOpen, window_flags))
		{
			if (!opt_padding)
			{
				ImGui::PopStyleVar();
			}

			if (opt_fullscreen)
			{
				ImGui::PopStyleVar(2);
			}

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
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}

					if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
					{
						SaveSceneAs();
					}

					if (ImGui::MenuItem("Exit"))
					{
						Application::Get().Close();
					}

					ImGui::Separator();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_sceneHierachyPanel.OnImGuiRender();
			m_contentBrowserPanel.OnImGuiRender();

			if (ImGui::Begin("Settings"))
			{
				auto stats = Renderer2D::GetState();
				ImGui::Text("Renderer stats");
				ImGui::Text("Draw Calls: %d", stats.drawCalls);
				ImGui::Text("Quads: %d", stats.quadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

				ImGui::End();
			}

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin("Viewport"))
			{
				auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
				auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
				auto viewportOffset = ImGui::GetWindowPos();
				m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
				m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

				m_viewportFocused = ImGui::IsWindowFocused();
				m_viewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->DisableEvents(m_viewportFocused || m_viewportHovered);

				ImVec2 viewportSize = ImGui::GetContentRegionAvail();
				m_viewportSize = glm::vec2(viewportSize.x, viewportSize.y);

				uint32_t textureId = m_framebuffer->GetColorAttachmentRendererId(0);
				ImGui::Image((void*)textureId, ImVec2(m_viewportSize.x, m_viewportSize.y), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						OpenScene(path);
					}
				}

				// ImGuizmo
				Entity selectedEntity = m_sceneHierachyPanel.GetSelectedEntity();
				if (selectedEntity && m_gizmoType != -1)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();
					ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);
			
					// Camera
					//auto cameraEntity = m_activeScene->GetPrimaryCameraEntity();
					//const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
					//const glm::mat4& cameraProjection = camera.GetProjection();
					//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

					// Editor camera
					const glm::mat4& cameraProjection = m_editorCamera.GetProjection();
					glm::mat4 cameraView = m_editorCamera.GetViewMatrix();

					// Entity transform
					auto& tc = selectedEntity.GetComponent<TransformComponent>();
					glm::mat4 transform = tc.GetTransform();

					// Snapping
					bool snap = Input::IsKeyPressed(ENGINE_KEY_LEFT_CONTROL);
					float snapValue = 0.5f; // 0.5 meter for translation/scale
					if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
					{
						snapValue = 45.0f; // 45.0 degrees for rotation
					}

					float snapValues[3] = { snapValue, snapValue, snapValue };

					ImGuizmo::Manipulate(
						glm::value_ptr(cameraView),
						glm::value_ptr(cameraProjection),
						(ImGuizmo::OPERATION)m_gizmoType,
						ImGuizmo::LOCAL,
						glm::value_ptr(transform),
						nullptr,
						snap ? snapValues : nullptr
					);

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Math::DecomposeTransform(transform, translation, rotation, scale);

						glm::vec3 deltaRotation = rotation - tc.rotation;
						tc.translation = translation;
						tc.rotation += deltaRotation;
						tc.scale = scale;
					}
				}

				ImGui::End();
			}
			ImGui::PopStyleVar();
			ImGui::End();
		}
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

		m_editorCamera.OnEvent(event);
		m_cameraController.OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(ENGINE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ENGINE_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		bool control = Input::IsKeyPressed(ENGINE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(ENGINE_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(ENGINE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(ENGINE_KEY_RIGHT_SHIFT);

		switch (event.GetKeyCode())
		{
		case ENGINE_KEY_N:
			if (control)
			{
				NewScene();
			}
			break;
		case ENGINE_KEY_O:
			if (control)
			{
				OpenScene();
			}
			break;
		case ENGINE_KEY_S:
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;
		case ENGINE_KEY_Q:
			m_gizmoType = -1;
			break;
		case ENGINE_KEY_W:
			m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case ENGINE_KEY_E:
			m_gizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		case ENGINE_KEY_R:
			m_gizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		default:
			break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) 
	{
		if (event.GetMouseButton() == ENGINE_MOUSE_BUTTON_LEFT)
		{
			if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(ENGINE_KEY_LEFT_ALT))
			{
				m_sceneHierachyPanel.SetSelectedEntity(m_hoverdEntity);
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_activeScene = CreatePtr<Scene>();
		m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierachyPanel.SetContext(m_activeScene);
	}
	
	void EditorLayer::OpenScene()
	{
		std::string& filepath = FileDialogs::OpenFile("Game scene (*.xengine)\0*.xengine\0");

		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& filepath)
	{
		m_activeScene = CreatePtr<Scene>();
		m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_sceneHierachyPanel.SetContext(m_activeScene);

		SceneSerializer serializer(m_activeScene);
		serializer.Deserialize(filepath.string());
	}
	
	void EditorLayer::SaveSceneAs()
	{
		std::string& filepath = FileDialogs::SaveFile("Game scene (*.xengine)\0*.xengine\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(m_activeScene);
			serializer.Serialize(filepath);
		}
	}
}
