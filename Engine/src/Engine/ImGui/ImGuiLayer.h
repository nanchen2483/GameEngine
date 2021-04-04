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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_time = 0.0f;
	};
}

