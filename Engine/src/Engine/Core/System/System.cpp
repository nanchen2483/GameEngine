#include "System.h"

namespace Engine
{
	glm::vec2 System::s_mousePosition = glm::vec2(0.0f);
	glm::vec2 System::s_deltaMousePosition = glm::vec2(0.0f);
	float System::s_mouseVelocity = 0.003f;

	void System::OnUpdate()
	{
		OnMouseUpdate();
	}
	
	void System::OnMouseUpdate()
	{
		const glm::vec2& currentMouse{ Input::GetMouseX(), Input::GetMouseY() };
		s_deltaMousePosition = (currentMouse - s_mousePosition) * s_mouseVelocity;
		s_mousePosition = currentMouse;
	}
}
