#pragma once
#include "FramebufferAttachmentSpecification.h"

namespace Engine
{
	struct FramebufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		uint32_t arraySize = 1;
		FramebufferAttachmentSpecification attachments;

		bool swapChainTarget = false;
	};
}
