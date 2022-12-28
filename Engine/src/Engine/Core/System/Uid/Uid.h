#pragma once
#include <string>

namespace Engine
{
	class Uid
	{
	public:
		Uid();
		Uid(const std::string& str);
		static Uid NewUid();
		static Uid NewUid(const std::string& str);

		operator uint64_t() const { return m_uid; }
	private:
		uint64_t m_uid;
	};
}
