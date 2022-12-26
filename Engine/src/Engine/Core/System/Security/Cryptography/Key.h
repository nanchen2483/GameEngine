#pragma once
#include <stdint.h>

namespace Engine
{
	class Key
	{
	public:
		Key(uint32_t key)
			: m_key(key)
		{
		}

		operator uint32_t() const { return m_key; }
	private:
		uint32_t m_key;
	};
}
