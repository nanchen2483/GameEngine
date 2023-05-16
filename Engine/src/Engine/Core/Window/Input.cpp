#include "enginepch.h"
#include "Input.h"
#include "Engine/Renderer/Graphics/IGraphicsLibrary.h"

namespace Engine
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		KeyMouseButtonActions state = IGraphicsLibrary::GetInstance().GetKeyState(keycode);
		return state == KeyMouseButtonActions::Press ||
			   state == KeyMouseButtonActions::Repeat;
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		return IGraphicsLibrary::GetInstance().GetMouseButtonState(button) == KeyMouseButtonActions::Press;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return IGraphicsLibrary::GetInstance().GetCursorPosition();
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();
		return x;
	}

	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();
		return y;
	}
	
	bool Input::IsCursorVisible()
	{
		return IGraphicsLibrary::GetInstance().GetCursorMode() == CursorMode::Normal;
	}

	void Input::ShowCursor()
	{
		IGraphicsLibrary::GetInstance().SetCursorMode(CursorMode::Normal);
	}
	
	void Input::HideCursor()
	{
		IGraphicsLibrary::GetInstance().SetCursorMode(CursorMode::Disabled);
	}
}
