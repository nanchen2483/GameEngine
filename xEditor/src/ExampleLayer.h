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
		Ptr<Scene> m_activeScene;
		EditorCamera m_editorCamera;

		Ptr<UniformBuffer> m_cameraUniformBuffer;
		Ptr<UniformBuffer> m_dirLightUniformBuffer;

		Ptr<Model> m_model;
		Ptr<Shader> m_shader;
		Ptr<Skybox> m_skybox;
	};
}
