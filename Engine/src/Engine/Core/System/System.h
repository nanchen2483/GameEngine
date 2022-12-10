#pragma once
#include "Engine/Core/Window/Input.h"

#include <glm/glm.hpp>

namespace Engine
{
	class System
	{
	public:
		static glm::vec2 GetMouseDeltaPosition() { return s_deltaMousePosition; }

		static void OnUpdate();
	private:
		static void OnMouseUpdate();

		static glm::vec2 s_mousePosition;
		static glm::vec2 s_deltaMousePosition;
		static float s_mouseVelocity;
	};
}
