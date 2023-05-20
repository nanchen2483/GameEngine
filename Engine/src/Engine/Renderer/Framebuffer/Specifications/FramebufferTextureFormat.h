#pragma once

namespace Engine
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RedInteger,
		RGBA8,
		RGBA16,
		RGBA32,
		Depth24Stencil8,
		DepthArray,
		Depth = Depth24Stencil8,
	};
}
