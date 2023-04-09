#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	class Vector3DHash
	{
	public:
		size_t operator()(const glm::dvec3& v) const
		{
			size_t hash = 0;
			hash_combine(hash, std::hash<double>{}(v.x));
			hash_combine(hash, std::hash<double>{}(v.y));
			hash_combine(hash, std::hash<double>{}(v.z));
			return hash;
		}
	private:
		template <class T>
		static inline void hash_combine(size_t& seed, const T& val)
		{
			seed ^= std::hash<T>{}(val)+0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	};
}
