#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Core/TimeStep.h"

namespace Engine
{
	class ENGINE_API Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep timeStep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_debugName; }

	protected:
		std::string m_debugName;
	};
}

