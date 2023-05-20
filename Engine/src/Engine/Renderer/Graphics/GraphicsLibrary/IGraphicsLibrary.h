#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/Enum/CursorMode.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/KeyMouseButtonActions.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"
#include "Engine/Core/Window/WindowUserData.h"
#include "Engine/Renderer/Graphics/GraphicsContext/IGraphicsContext.h"

namespace Engine
{
	class IGraphicsLibrary
	{
	public:
		virtual void CreateNewWindow(std::string title, uint32_t width, uint32_t height) = 0;
		virtual void* GetWindow() const = 0;
		virtual void* GetWin32Window() const = 0;
		virtual void DestroyWindow() = 0;
		virtual Uniq<IGraphicsContext> GetContext() = 0;
		virtual void SetWindowUserDataPointer(WindowUserData *userData) = 0;
		virtual WindowUserData* GetWindowUserDataPointer() const = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual void PollEvents() = 0;
		virtual KeyMouseButtonActions GetKeyState(KeyCode keycode) const = 0;
		virtual KeyMouseButtonActions GetMouseButtonState(MouseButton button) const = 0;
		virtual std::pair<float, float> GetCursorPosition() const = 0;
		virtual CursorMode GetCursorMode() const = 0;
		virtual void SetCursorMode(CursorMode mode) = 0;
		virtual void SetWindowsSizeCallback(void (*callback)(void* window, int32_t width, int32_t height)) = 0;
		virtual void SetWindowCloseCallback(void (*callback)(void* window)) = 0;
		virtual void SetKeyCallback(void (*callback)(void* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)) = 0;
		virtual void SetCharCallback(void (*callback)(void* window, uint32_t keycode)) = 0;
		virtual void SetMouseButtonCallback(void (*callback)(void* window, int32_t button, int32_t action, int32_t mods)) = 0;
		virtual void SetScrollCallback(void (*callabck)(void* window, double xOffset, double yOffset)) = 0;
		virtual void SetCursorPosCallback(void (*callback)(void* window, double xPos, double yPos)) = 0;
	};
}
