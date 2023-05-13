#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Events/Event.h"

namespace Engine
{
	class ENGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnImGuiRender() = 0;
		virtual void OnEvent(Event& event) = 0;

		inline const std::string& GetName() const { return m_name; }
	protected:
		std::string m_name;
	};
}

