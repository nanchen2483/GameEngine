#pragma once
#include "Engine/Core/Enum/GraphicsApiType.h"
#include "Engine/Core/Enum/OperatingSystem.h"

#include <chrono>
#include <glm/glm.hpp>

namespace Engine
{
	class ENGINE_API System
	{
	public:
		inline static OperatingSystem GetOperatingSystem() { return s_operatingSystem; }
		inline static GraphicsApiType GetGraphicsApiType() { return s_graphicsAPI; }
		static double GetDeltaTime() { return s_deltaTime; }
		static glm::vec2 GetMouseDeltaPosition() { return s_deltaMousePosition; }
		static float GetFreeFallVelocity(float fallingTime) { return s_gravitationalAcceleration * fallingTime; }
	private:
		static void OnUpdate();
		static void OnTimeUpdate();
		static void OnMouseUpdate();

		static OperatingSystem s_operatingSystem;
		static GraphicsApiType s_graphicsAPI;
		static std::chrono::system_clock::time_point s_time;
		static double s_deltaTime;
		static glm::vec2 s_mousePosition;
		static glm::vec2 s_deltaMousePosition;
		static float s_mouseVelocity;
		static float s_gravitationalAcceleration;

		friend class Application;
	};
}
