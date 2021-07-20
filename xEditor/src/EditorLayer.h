#pragma once

#include "Engine.h"
#include "Panel/SceneHierarchyPanel.h"

namespace Engine
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(TimeStep timeStep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		OrthographicCameraController m_cameraController;
		Ptr<Texture2D> m_texture2D;
		Ptr<Framebuffer> m_framebuffer;
		glm::vec4 m_color;
		glm::vec2 m_viewportSize = glm::vec2(0.0f);
		bool m_viewportFocused = false, m_viewportHovered = false;
		Ptr<Scene> m_activeScene;
		Entity m_squareEntity;
		bool m_isPrimaryCamera = true;
		EditorCamera m_editorCamera;
		int m_gizmoType = -1;
		SceneHierarchyPanel m_sceneHierachyPanel;

		Entity m_hoverdEntity;
		glm::vec2 m_viewportBounds[2];
	};
}