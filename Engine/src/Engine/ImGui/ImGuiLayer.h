#pragma once

#include "Engine/Core/Layer.h"

#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"

namespace Engine
{
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		void OnAttach();
		void OnDetach();
		void OnEvent(Event& e);
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void DisableEvents(bool disable) { m_disableEvents = disable; }
	private:
		float m_time = 0.0f;
		bool m_disableEvents = false;
	};
}

