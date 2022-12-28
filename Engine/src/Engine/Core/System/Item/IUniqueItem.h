#pragma once
#include "Engine/Core/System/Uid/Uid.h"

namespace Engine
{
	class IUniqueItem
	{
	public:
		virtual Uid GetUid() const = 0;
	};
}
