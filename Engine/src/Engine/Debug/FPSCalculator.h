#pragma once
#include <cstdint>
#include <chrono>

namespace Engine
{
	class FPSCalculator
	{
	public:
		void Update()
		{
			m_currentTime = std::chrono::system_clock::now();
			m_frameCount++;
			std::chrono::duration<double> elapsedSeconds = m_currentTime - m_previousTime;
			if (elapsedSeconds.count() >= 1.0f)
			{
				m_FPS = m_frameCount / elapsedSeconds.count();
				m_frameCount = 0;
				m_previousTime = m_currentTime;
			}
		}

		float GetFPS()
		{
			return m_FPS;
		}

	private:
		std::chrono::system_clock::time_point m_previousTime = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point m_currentTime;
		uint32_t m_frameCount = 0;

		float m_FPS = 0;
	};
}
