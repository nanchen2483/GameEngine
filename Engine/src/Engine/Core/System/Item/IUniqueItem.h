#pragma once
#include <cstdint>

namespace Engine
{
	class IUniqueItem
	{
	public:
		virtual uint32_t GetId() const = 0;
	};
}
