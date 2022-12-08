#pragma once
#include "Engine.h"
#include "Gui/Dockspace.h"
#include "Gui/Sidebar/Menubar.h"
#include "Gui/Sidebar/Statusbar.h"
#include "Gui/Sidebar/Toolbar.h"

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

		Ptr<Framebuffer> m_framebuffer;
		glm::vec2 m_viewportSize = glm::vec2(1280.0f, 720.0f);
		bool m_viewportFocused = false, m_viewportHovered = false;
		Ptr<Scene> m_activeScene;
		EditorCamera m_editorCamera;
		int32_t m_guizmoType = -1;

		Entity m_hoveredEntity;
		glm::vec2 m_viewportBounds[2] = { glm::vec2(0.0f), glm::vec2(0.0f) };

		FPSCalculator m_FPSCalculator;
		int32_t m_textureId = -1;
		
		// Gui
		Dockspace m_dockspace;
		Menubar m_menubar;
		Toolbar m_toolbar;
		Statusbar m_statusbar;
	};
}
