#pragma once

#include "Engine.h"

class Sandbox2D : public Engine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Engine::TimeStep timeStep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;
private:
	Engine::OrthographicCameraController m_cameraController;
	Engine::Ptr<Engine::Texture2D> m_texture2D;
	Engine::Ptr<Engine::Framebuffer> m_framebuffer;
	glm::vec4 m_color;
};