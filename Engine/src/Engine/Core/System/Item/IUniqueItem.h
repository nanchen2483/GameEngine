#pragma once
#include <cstdint>

namespace Engine
{
	class IUniqueItem
	{
	public:
		virtual uint64_t GetUid() const = 0;
	};
}
