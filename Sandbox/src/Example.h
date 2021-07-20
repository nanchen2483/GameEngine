#pragma once

#include "Engine.h"

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Engine::TimeStep timeStep) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Engine::Event& event) override;
private:
	Engine::OrthographicCameraController m_cameraController;

	Engine::ShaderLibrary m_shaderLibrary;
	Engine::Ptr<Engine::VertexArray> m_vertexArray;
	Engine::Ptr<Engine::Texture2D> m_texture2D;

	glm::vec3 m_color;
};