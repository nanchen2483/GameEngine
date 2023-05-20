#pragma once
#include <vector>

namespace Engine
{
	struct ShadowBoxInfo
	{
		const uint32_t depthTextureSlot = 40;
		const float nearPlane = 0.1f;
		const float farPlane = 1000.0f;
		const std::vector<float> levels
		{
			farPlane / 50.0f,
			farPlane / 25.0f,
			farPlane / 10.0f,
			farPlane / 2.0f,
			farPlane
		};
	};
}
