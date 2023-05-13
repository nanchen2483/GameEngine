#include "enginepch.h"
#include "FPSCalculator.h"

namespace Engine
{
	FPSCalculator::FPSCalculator()
		: m_FPS(0.0f), m_frameCount(0), m_previousTime(std::chrono::system_clock::now())
	{
	}

	void FPSCalculator::Update()
	{
		m_currentTime = std::chrono::system_clock::now();
		m_frameCount++;
		std::chrono::duration<double> elapsedSeconds = m_currentTime - m_previousTime;
		if (elapsedSeconds.count() >= 1.0f)
		{
			m_FPS = (float)m_frameCount / (float)elapsedSeconds.count();
			m_frameCount = 0;
			m_previousTime = m_currentTime;
		}
	}
}
