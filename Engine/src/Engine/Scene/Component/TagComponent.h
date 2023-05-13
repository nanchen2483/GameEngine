#pragma once
#include "IComponent.h"

#include <string>

namespace Engine
{
	struct ENGINE_API TagComponent : public IComponent
	{
		std::string tag;
	};
}
