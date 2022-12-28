#pragma once
#include "Engine/Core/System/Uid/Uid.h"

namespace Engine
{
	class IUniqueObject
	{
	public:
		virtual Uid GetUid() const = 0;
	};
}
