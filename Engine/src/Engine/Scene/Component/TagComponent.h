#pragma once
#include "IComponent.h"

#include <string>

namespace Engine
{
	struct TagComponent : public IComponent
	{
		std::string tag;
	};
}
