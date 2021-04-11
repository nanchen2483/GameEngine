#pragma once

#include "Engine.h"

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
	};
}