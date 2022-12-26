#pragma once
#include <stdint.h>

namespace Engine
{
	class Key
	{
	public:
		Key(uint64_t key)
			: m_key(key)
		{
		}

		operator uint64_t() const { return m_key; }
	private:
		uint64_t m_key;
	};
}
