#pragma once

namespace Engine
{
	template <typename T>
	class IComparable
	{
	public:
		virtual bool operator==(const T& other) const = 0;
		virtual bool operator!=(const T& other) const = 0;
		virtual bool operator<(const T& other) const = 0;
		virtual bool operator<=(const T& other) const = 0;
		virtual bool operator>(const T& other) const = 0;
		virtual bool operator>=(const T& other) const = 0;
	};
}
