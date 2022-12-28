#pragma once
#include <chrono>
#include <glm/glm.hpp>

namespace Engine
{
	class System
	{
	public:
		static double GetDeltaTime() { return s_deltaTime; }
		static glm::vec2 GetMouseDeltaPosition() { return s_deltaMousePosition; }
	private:
		static void OnUpdate();
		static void OnTimeUpdate();
		static void OnMouseUpdate();

		static std::chrono::system_clock::time_point s_time;
		static double s_deltaTime;
		static glm::vec2 s_mousePosition;
		static glm::vec2 s_deltaMousePosition;
		static float s_mouseVelocity;

		friend class Application;
	};
}
