#pragma once

#include "Engine.h"

namespace Engine
{
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
		OrthographicCameraController m_cameraController;
		Ptr<Scene> m_activeScene;
		EditorCamera m_editorCamera;

		ShaderLibrary m_shaderLibrary;
		Ptr<Engine::VertexArray> m_vertexArray;
		Ptr<Engine::Texture2D> m_texture2D;

		glm::vec3 m_color;
		Ptr<IModel> m_model;
		Ptr<Shader> m_shader;
	};
}
