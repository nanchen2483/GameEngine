#include "enginepch.h"
#include "Input.h"
#include "Engine/Factory/GraphicsFactory.h"

namespace Engine
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		KeyMouseButtonActions state = GraphicsFactory::GetLibraryInstance().GetKeyState(keycode);
		return state == KeyMouseButtonActions::Press ||
			   state == KeyMouseButtonActions::Repeat;
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		return GraphicsFactory::GetLibraryInstance().GetMouseButtonState(button) == KeyMouseButtonActions::Press;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		return GraphicsFactory::GetLibraryInstance().GetCursorPosition();
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
		return GraphicsFactory::GetLibraryInstance().GetCursorMode() == CursorMode::Normal;
	}

	void Input::ShowCursor()
	{
		GraphicsFactory::GetLibraryInstance().SetCursorMode(CursorMode::Normal);
	}
	
	void Input::HideCursor()
	{
		GraphicsFactory::GetLibraryInstance().SetCursorMode(CursorMode::Disabled);
	}
}
