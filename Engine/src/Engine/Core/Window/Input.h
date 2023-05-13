#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"

#include <utility>

namespace Engine
{
	class ENGINE_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseButton button);
		static std::pair<float, float> GetMousePosition();
		
		static float GetMouseX();
		static float GetMouseY();
		
		static bool IsCursorVisible();
		static void ShowCursor();
		static void HideCursor();
	};
}
