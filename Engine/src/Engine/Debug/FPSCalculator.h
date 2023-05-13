#pragma once
#include "Engine/Core/Base.h"

#include <chrono>

namespace Engine
{
	class ENGINE_API FPSCalculator
	{
	public:
		FPSCalculator();

		void Update();
		float GetFPS() const { return m_FPS; }
	private:
		float m_FPS;
		uint32_t m_frameCount;
		std::chrono::system_clock::time_point m_previousTime;
		std::chrono::system_clock::time_point m_currentTime;
	};
}
