#pragma once
#include <vector>

namespace Engine
{
	struct ShadowBoxData
	{
		static const uint32_t depthTextureSlot = 40;
		static constexpr float nearPlane = 0.1f;
		static constexpr float farPlane = 1000.0f;
		static const uint32_t numOfLevels = 6;
		static const uint32_t numOfCascades = numOfLevels - 1;
		static constexpr float levels[numOfLevels]
		{
			nearPlane,
			farPlane / 50.0f,
			farPlane / 25.0f,
			farPlane / 10.0f,
			farPlane / 2.0f,
			farPlane
		};
	};
}
