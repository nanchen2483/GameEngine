#pragma once

namespace Engine
{
	enum class UniformBufferBindingPoint : uint32_t
	{
		CameraBlock = 0,
		DirLightBlock = 1,
		PointLightBlock = 2,
		LightSpaceBlock = 3,
		TransformBlock = 4,
	};
}
