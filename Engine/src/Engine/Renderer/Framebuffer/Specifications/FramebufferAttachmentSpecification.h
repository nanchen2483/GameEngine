#pragma once
#include "FramebufferTextureSpecification.h"

#include <vector>

namespace Engine
{
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::vector<FramebufferTextureSpecification> attachments)
			: attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> attachments{};
	};
}
