#pragma once
#include "Engine/Core/Constant/CameraConstant.h"

#include <vector>

namespace Engine
{
	struct ShadowBoxData
	{
		static const uint32_t depthTextureSlot = 40;
		static const uint32_t numOfLevels = 6;
		static const uint32_t numOfCascades = numOfLevels - 1;
		static constexpr float levels[numOfLevels]
		{
			CameraConstant::NearPlane,
			CameraConstant::FarPlane / 50.0f,
			CameraConstant::FarPlane / 25.0f,
			CameraConstant::FarPlane / 10.0f,
			CameraConstant::FarPlane / 2.0f,
			CameraConstant::FarPlane,
		};
	};
}
