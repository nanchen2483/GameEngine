#include "enginepch.h"
#include "System.h"
#include "Engine/Core/Window/Input.h"

namespace Engine
{
#ifdef ENGINE_PLATFORM_WINDOWS
	OperatingSystem System::s_operatingSystem = OperatingSystem::Windows;
#else
	#error Engine only support windows
#endif // ENGINE_PLATFORM_WINDOWS

#ifdef ENGINE_GRAPHICS_API_OPENGL
	GraphicsApiType System::s_graphicsAPI = GraphicsApiType::OpenGL;
#else
	#error Engine only support OpenGL API
#endif

	std::chrono::system_clock::time_point System::s_time = std::chrono::system_clock::now();
	double System::s_deltaTime = 0.0f;
	glm::vec2 System::s_mousePosition = glm::vec2(0.0f);
	glm::vec2 System::s_deltaMousePosition = glm::vec2(0.0f);
	float System::s_mouseVelocity = 0.003f;
	float System::s_gravitationalAcceleration = 9.8f;
	void System::OnUpdate()
	{
		OnTimeUpdate();
		OnMouseUpdate();
	}

	void System::OnTimeUpdate()
	{
		std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsedSeconds = currentTime - s_time;
		s_deltaTime = elapsedSeconds.count();
		s_time = currentTime;
	}

	void System::OnMouseUpdate()
	{
		const glm::vec2& currentMouse{ Input::GetMouseX(), Input::GetMouseY() };
		s_deltaMousePosition = (currentMouse - s_mousePosition) * s_mouseVelocity;
		s_mousePosition = currentMouse;
	}
}
