#pragma once
#include "Engine.h"
#include "Engine/Factory/BufferFactory.h"

namespace Engine
{
	class ExampleLayer : public Layer
	{
	public:
		ExampleLayer();
		virtual ~ExampleLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Engine::Event& event) override;
	private:
		EditorCamera m_editorCamera;
		Ptr<ITerrain> m_terrain;
		Ptr<IUniformBuffer> m_cameraUniformBuffer;
	};
}
