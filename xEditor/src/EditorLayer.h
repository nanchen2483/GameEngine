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
		OrthographicCameraController m_cameraController;
		Ptr<Texture2D> m_texture2D;
		Ptr<Framebuffer> m_framebuffer;
		glm::vec4 m_color;
		glm::vec2 m_viewportSize = glm::vec2(0.0f);
		bool m_viewportFocused = false, m_viewportHovered = false;
		Ptr<Scene> m_activeScene;
		Entity m_squareEntity;
		Entity m_cameraEntity;
		Entity m_secondCameraEntity;
		bool m_isPrimaryCamera = true;
		SceneHierarchyPanel m_sceneHierachyPanel;
	};
}