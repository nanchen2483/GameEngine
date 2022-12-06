#include "EditorLayer.h"

#include "Engine/Debug/Instrumentor.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <ImGuizmo.h>

#define ENTITY_ID_ATTACHMENT_INDEX 1

namespace Engine
{
	EditorLayer::EditorLayer()
		: Layer("EditorLayer")
	{
	}

	void EditorLayer::OnAttach()
	{
		ENGINE_PROFILE_FUNCTION();

		FramebufferSpecification fbSpec;
		fbSpec.attachments = FramebufferAttachmentSpecification(
			{
				FramebufferTextureFormat::RGBA8, // Main framebuffer
				FramebufferTextureFormat::RED_INTEGER, // EntityId
				FramebufferTextureFormat::DEPTH24STENCIL8
			});
		fbSpec.width = 1280;
		fbSpec.height = 720;
		m_framebuffer = Framebuffer::Create(fbSpec);

		// Entity
		m_activeScene = CreatePtr<Scene>();

		m_editorCamera = EditorCamera(45.0f, m_viewportSize.x, m_viewportSize.y, 0.1f, 10000.0f);

		m_menubar.GetHierarchy()->SetContext(m_activeScene);
		m_menubar.SetFunctions(
			ENGINE_BIND_FN(EditorLayer::NewScene),
			ENGINE_BIND_FN(EditorLayer::OpenScene),
			ENGINE_BIND_FN(EditorLayer::SaveSceneAs));
		m_toolbar.SetFunctions(
			ENGINE_BIND_FN(EditorLayer::NewScene),
			ENGINE_BIND_FN(EditorLayer::OpenScene),
			ENGINE_BIND_FN(EditorLayer::SaveSceneAs));
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(TimeStep timeStep)
	{
		ENGINE_PROFILE_FUNCTION();

		m_FPSCalculator.Update();

		if (FramebufferSpecification spec = m_framebuffer->GetSpecification();
			m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != m_viewportSize.x || spec.height != m_viewportSize.y))
		{
			m_framebuffer->Resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_editorCamera.SetViewportSize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
			m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		}

		if (m_viewportHovered && !m_toolbar.IsPlaying())
		{
			ENGINE_PROFILE_SCOPE("Camera OnUpdate");
			m_editorCamera.OnUpdate(timeStep);
		}

		Renderer3D::ResetStates();
		m_framebuffer->Bind();
		RendererCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		RendererCommand::Clear();

		// Clear entity id buffer attachment to -1
		m_framebuffer->ClearAttachment(ENTITY_ID_ATTACHMENT_INDEX, -1);

		if (m_toolbar.IsPlaying())
		{
			m_activeScene->OnUpdateRuntime(timeStep);
		}
		else
		{
			m_activeScene->OnUpdateEditor(timeStep, m_editorCamera);
		}

		m_framebuffer->Bind();
		UpdateHoveredEntity();
		m_framebuffer->Unbind();
	}

	void EditorLayer::UpdateHoveredEntity()
	{
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_viewportBounds[0].x;
		my -= m_viewportBounds[0].y;
		glm::vec2 viewportSize = { m_viewportBounds[1] - m_viewportBounds[0] };
		my = m_viewportSize.y - my;

		int mouseX = (int)mx; 
		int mouseY = (int)my;
		if (mouseX >  0 && mouseY > 0 && mouseX < viewportSize.x && mouseY < viewportSize.y)
		{
			int pixelData = m_framebuffer->ReadPixel(ENTITY_ID_ATTACHMENT_INDEX, mouseX, mouseY);
			if (pixelData == -1)
			{
				m_hoveredEntity = Entity();
			}
			else
			{
				entt::entity entity = (entt::entity)pixelData;
				if (m_activeScene->EntityExists(entity))
				{
					m_hoveredEntity = Entity(entity, m_activeScene.get());
				}
			}
		}
	}

	void EditorLayer::OnImGuiRender()
	{
		ENGINE_PROFILE_FUNCTION();

		m_dockspace.OnImGuiRender(
			[&]()
			{
				m_menubar.OnImGuiRender();
				m_toolbar.OnImGuiRender();
				m_statusbar.OnImGuiRender();

				ImGuiWindowClass windowClass;
				windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::SetNextWindowClass(&windowClass);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				if (ImGui::Begin("Viewport"))
				{
					auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
					auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
					auto viewportOffset = ImGui::GetWindowPos();
					m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
					m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

					m_viewportFocused = ImGui::IsWindowFocused() || m_editorCamera.IsCursorInsideViewport();
					m_viewportHovered = ImGui::IsWindowHovered() || m_editorCamera.IsCursorInsideViewport();
					Application::Get().GetImGuiLayer()->BlockEvents(!m_viewportFocused || !m_viewportHovered);

					ImVec2 viewportSize = ImGui::GetContentRegionAvail();
					m_viewportSize = glm::vec2(viewportSize.x, viewportSize.y);

					uint64_t textureId = m_framebuffer->GetColorAttachmentRendererId(0);
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
					Entity selectedEntity = m_menubar.GetHierarchy()->GetSelectedEntity();
					if (selectedEntity && m_gizmoType != -1)
					{
						ImGuizmo::SetOrthographic(false);
						ImGuizmo::SetDrawlist();
						ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_viewportBounds[1].x - m_viewportBounds[0].x, m_viewportBounds[1].y - m_viewportBounds[0].y);

						// Editor camera
						const glm::mat4& cameraProjection = m_editorCamera.GetProjection();
						glm::mat4 cameraView = m_editorCamera.GetViewMatrix();

						// Entity transform
						Transform& transform = selectedEntity.GetComponent<TransformComponent>();
						glm::mat4 transformMatrix = transform;

						// Snapping
						bool snap = Input::IsKeyPressed(KeyCode::LEFT_CONTROL);
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
							glm::value_ptr(transformMatrix),
							nullptr,
							snap ? snapValues : nullptr
						);

						if (ImGuizmo::IsUsing())
						{
							glm::vec3 translation, rotation, scale;
							Math::DecomposeTransform(transformMatrix, translation, rotation, scale);

							glm::vec3 deltaRotation = rotation - transform.rotation;
							transform.translation = translation;
							transform.rotation += deltaRotation;
							transform.scale = scale;
						}
					}
				}
				ImGui::End();
				ImGui::PopStyleVar();
			});
	}

	void EditorLayer::OnEvent(Event& event)
	{
		if (event.GetEventType() == EventType::WindowResize)
		{
			WindowResizeEvent* e = (WindowResizeEvent*)&event;
			if (e->GetWidth() == 0 || e->GetHeight() == 0)
			{
				m_viewportSize = glm::vec2(0.0f);
			}
		}

		m_editorCamera.OnEvent(event);
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(ENGINE_BIND_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ENGINE_BIND_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
	{
		bool control = Input::IsKeyPressed(KeyCode::LEFT_CONTROL) || Input::IsKeyPressed(KeyCode::RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(KeyCode::LEFT_SHIFT) || Input::IsKeyPressed(KeyCode::RIGHT_SHIFT);
		bool cameraInUse = m_editorCamera.InUse();

		switch (event.GetKeyCode())
		{
		case KeyCode::N:
			if (control)
			{
				NewScene();
			}
			break;
		case KeyCode::O:
			if (control)
			{
				OpenScene();
			}
			break;
		case KeyCode::S:
			if (control && shift)
			{
				SaveSceneAs();
			}
			break;
		case KeyCode::Q:
			if (!cameraInUse)
			{
				m_gizmoType = -1;
			}
			break;
		case KeyCode::W:
			if (!cameraInUse)
			{
				m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
			break;
		case KeyCode::E:
			if (!cameraInUse)
			{
				m_gizmoType = ImGuizmo::OPERATION::SCALE;
			}
			break;
		case KeyCode::R:
			if (!cameraInUse)
			{
				m_gizmoType = ImGuizmo::OPERATION::ROTATE;
			}
			break;
		case KeyCode::F:
			if (m_hoveredEntity)
			{
				Transform& transform = m_hoveredEntity.GetComponent<TransformComponent>();
				m_editorCamera.UpdateFocusPoint(transform);
			}
			break;
		default:
			break;
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) 
	{
		if (event.GetMouseButton() == MouseButton::LEFT_BUTTON)
		{
			if (m_viewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LEFT_ALT))
			{
				m_menubar.GetHierarchy()->SetSelectedEntity(m_hoveredEntity);
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_activeScene = CreatePtr<Scene>();
		m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_menubar.GetHierarchy()->SetContext(m_activeScene);
	}
	
	void EditorLayer::OpenScene()
	{
		const std::string& filepath = FileDialogs::OpenFile("Game scene (*.xengine)\0*.xengine\0");

		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& filepath)
	{
		m_activeScene = CreatePtr<Scene>();
		m_activeScene->OnViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
		m_menubar.GetHierarchy()->SetContext(m_activeScene);

		SceneSerializer serializer(m_activeScene);
		serializer.Deserialize(filepath.string());
	}
	
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Game scene (*.xengine)\0*.xengine\0");

		if (!filepath.empty())
		{
			std::filesystem::path extension = std::filesystem::path(filepath).extension();
			if (extension != ".xengine")
			{
				filepath += ".xengine";
			}

			SceneSerializer serializer(m_activeScene);
			serializer.Serialize(filepath);
		}
	}
}
