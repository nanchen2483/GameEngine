#pragma once
#include "Engine/Core/Constant/CameraConstant.h"
#include "FramebufferAttachmentSpecification.h"

namespace Engine
{
	struct FramebufferSpecification
	{
		uint32_t width{ (uint32_t)CameraConstant::ViewportWidth };
		uint32_t height{ (uint32_t)CameraConstant::ViewportHeight };
		uint32_t samples{ 1 };
		uint32_t arraySize{ 1 };
		FramebufferAttachmentSpecification attachments{};

		bool swapChainTarget{ false };
	};
}
