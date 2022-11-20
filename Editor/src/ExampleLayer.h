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
		EditorCamera m_editorCamera;
		Ptr<Terrain> m_terrain;
		Ptr<UniformBuffer> m_cameraUniformBuffer;
	};
}
