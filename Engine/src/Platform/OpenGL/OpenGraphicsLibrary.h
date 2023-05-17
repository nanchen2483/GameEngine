#pragma once
#include "Engine/Renderer/Graphics/IGraphicsLibrary.h"
#include "Engine/Renderer/Graphics/IGraphicsContext.h"

#include <string>

struct GLFWwindow;

namespace Engine
{
	class OpenGraphicsLibrary : public IGraphicsLibrary
	{
	public:
		OpenGraphicsLibrary();
		~OpenGraphicsLibrary();

		virtual void CreateNewWindow(std::string title, uint32_t width, uint32_t height) override;
		virtual void* GetWindow() const override { return m_window; };
		virtual void* GetWin32Window() const override;
		virtual void DestroyWindow() override;
		virtual Uniq<IGraphicsContext> GetContext() override { return std::move(m_context); };
		virtual void SetWindowUserDataPointer(WindowUserData* userData) override;
		virtual WindowUserData* GetWindowUserDataPointer() const override;
		virtual void SetVSync(bool enable) override;
		virtual void PollEvents() override;

		virtual KeyMouseButtonActions GetKeyState(KeyCode keycode) const override;
		virtual KeyMouseButtonActions GetMouseButtonState(MouseButton button) const override;
		virtual std::pair<float, float> GetCursorPosition() const override;
		virtual CursorMode GetCursorMode() const override;
		virtual void SetCursorMode(CursorMode mode) override;

		virtual void SetWindowsSizeCallback(void (*callback)(void* window, int32_t width, int32_t height)) override;
		virtual void SetWindowCloseCallback(void (*callback)(void* window)) override;
		virtual void SetKeyCallback(void (*callback)(void* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)) override;
		virtual void SetCharCallback(void (*callback)(void* window, uint32_t keycode)) override;
		virtual void SetMouseButtonCallback(void (*callback)(void* window, int32_t button, int32_t action, int32_t mods)) override;
		virtual void SetScrollCallback(void (*callabck)(void* window, double xOffset, double yOffset)) override;
		virtual void SetCursorPosCallback(void (*callback)(void* window, double xPos, double yPos)) override;

		static WindowUserData* GetWindowUserDataPointerStatic(void* window);
	private:
		GLFWwindow* m_window;
		Uniq<IGraphicsContext> m_context;
	};
}
