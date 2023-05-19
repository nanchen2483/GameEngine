#pragma once

namespace Engine
{
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,
		RedInteger,
		Depth24Stencil8,
		DepthArray,
		Depth = Depth24Stencil8,
	};
}
