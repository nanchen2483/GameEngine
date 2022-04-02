#pragma once

#include "Engine.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/ContentBrowserPanel.h"

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
		void UpdateHoveredEntity();
		bool OnKeyPressed(KeyPressedEvent& event);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& filepath);
		void SaveSceneAs();
	private:
		Ptr<Framebuffer> m_framebuffer;
		glm::vec2 m_viewportSize = glm::vec2(0.0f);
		bool m_viewportFocused = false, m_viewportHovered = false;
		Ptr<Scene> m_activeScene;
		EditorCamera m_editorCamera;
		int m_gizmoType = -1;
		SceneHierarchyPanel m_sceneHierachyPanel;
		ContentBrowserPanel m_contentBrowserPanel;

		Entity m_hoveredEntity;
		glm::vec2 m_viewportBounds[2] = { glm::vec2(0.0f), glm::vec2(0.0f) };

		int m_textureId = -1;
	};
}
