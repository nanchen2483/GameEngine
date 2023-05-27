#pragma once
#include "Engine/Core/Constant/CameraProperty.h"

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
			CameraProperty::NearPlane,
			CameraProperty::FarPlane / 50.0f,
			CameraProperty::FarPlane / 25.0f,
			CameraProperty::FarPlane / 10.0f,
			CameraProperty::FarPlane / 2.0f,
			CameraProperty::FarPlane,
		};
	};
}
