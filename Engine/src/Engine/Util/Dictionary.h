#pragma once
#include <map>
#include <string>

namespace Engine
{
	template<typename K, typename V>
	class Dictionary
	{
	public:
		struct Value
		{
			uint32_t id;
			V value;
		};

		const Value& Get(const K key) const
		{
			auto iter = m_map.find(key);
			if (iter != m_map.end())
			{
				return iter->second;
			}

			ENGINE_CORE_ASSERT(false, "Dictionary key not found!");
		}

		const Value& Add(const K key, const V value)
		{
			if (!Exists(key))
			{
				m_map[key] = Value{ GetUniqueId(), value };
				return m_map[key];
			}

			ENGINE_CORE_ASSERT(false, "Dictionary key already exists!");
		}

		const Value& SafeAdd(const K key, const V value)
		{
			if (!Exists(key))
			{
				m_map[key] = Value{ GetUniqueId(), value };
			}

			return m_map[key];
		}

		bool Exists(const K key) const { return m_map.find(key) != m_map.end(); }
		const uint32_t Size() const { return m_size; }
	private:
		const uint32_t GetUniqueId() { return m_size++; }

		std::map<K, Value> m_map;
		uint32_t m_size = 0;
	};
}
