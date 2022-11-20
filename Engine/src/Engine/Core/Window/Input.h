#pragma once
#include "Engine/Core/Base.h"

#include <utility>

namespace Engine
{
	class ENGINE_API Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		
		static float GetMouseX();
		static float GetMouseY();
		
		static bool IsCursorVisible();
		static void ShowCursor();
		static void HideCursor();
	};
}
