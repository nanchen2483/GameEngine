#pragma once
#include "Engine/Core/Events/ApplicationEvent.h"
#include "Engine/Core/Events/KeyEvent.h"
#include "Engine/Core/Events/MouseEvent.h"
#include "Engine/Core/Layer/Layer.h"

#include <imgui_internal.h>

namespace Engine
{
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_blockEvents = block; }
		ImGuiContext* GetContext() const { return m_context; }
	private:
		float m_time = 0.0f;
		bool m_blockEvents = false;
		ImGuiContext* m_context = nullptr;
	};
}

