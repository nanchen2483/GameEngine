#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Core/System/Object/IComparable.h"

#include <type_traits>
#include <unordered_map>

namespace Engine
{
	template<typename T, typename std::enable_if_t<std::is_base_of_v<IComparable<T>, T>>* = nullptr>
	class OrderedLinkedList
	{
	public:
		OrderedLinkedList(uint32_t maxSize)
			: OrderedLinkedList()
		{
			ENGINE_CORE_ASSERT(maxSize >= 0, "The size of OrderedLinkedList cannot be negative");
			m_nodes.reserve(maxSize);
		}

		OrderedLinkedList()
			: m_head(nullptr)
			, m_tail(nullptr)
		{
		}

		const T& GetHeadValue() const
		{
			ENGINE_CORE_ASSERT(m_head != nullptr, "Linked list is empty.");
			return m_head->data;
		}

		const T& GetTailValue() const
		{
			ENGINE_CORE_ASSERT(m_tail != nullptr, "Linked list is empty.");
			return m_tail->data;
		}

		void Add(const T& data)
		{
			Uniq<Node> newNode = CreateUniq<Node>(data);
			if (m_head == nullptr)
			{
				m_head = newNode.get();
				m_tail = newNode.get();
			}
			else
			{
				// Stable sort
				Node* current = m_head;
				while (current != nullptr && *current <= data)
				{
					current = current->next;
				}

				if (current == nullptr)
				{
					newNode->prev = m_tail;
					m_tail->next = newNode.get();
					m_tail = newNode.get();
				}
				else if (current == m_head)
				{
					newNode->next = m_head;
					m_head->prev = newNode.get();
					m_head = newNode.get();
				}
				else
				{
					newNode->next = current;
					newNode->prev = current->prev;
					current->prev->next = newNode.get();
					current->prev = newNode.get();
				}
			}

			m_nodes.emplace_back(std::move(newNode));
		}

		void Delete(const T& data)
		{
			Node* current = m_head;
			while (current != nullptr && *current != data)
			{
				current = current->next;
			}

			if (current == nullptr)
			{
				ENGINE_CORE_ERROR("Data not found!");
				return;
			}
			else if (current == m_head)
			{
				m_head = current->next;
				if (m_head != nullptr)
				{
					m_head->prev = nullptr;
				}
				else
				{
					m_tail = nullptr;
				}
			}
			else if (current == m_tail)
			{
				m_tail = current->prev;
				m_tail->next = nullptr;
			}
			else
			{
				current->prev->next = current->next;
				current->next->prev = current->prev;
			}
		}

		class Iterator;
		Iterator begin() { return Iterator(m_head); }
		Iterator end() { return Iterator(nullptr); }
	private:
		struct Node;
		Node* m_head;
		Node* m_tail;
		std::vector<Uniq<Node>> m_nodes;

		struct Node
		{
			T data;
			Node* prev;
			Node* next;

			Node(const T& data)
				: data(data)
				, prev(nullptr)
				, next(nullptr)
			{
			}

			~Node()
			{
				if constexpr (is_smart_pointer_v<T>)
				{
					data.reset();
				}
				else if constexpr (std::is_pointer_v<T>)
				{
					delete data;
				}
			}

			bool operator<=(const T& other) const
			{
				if constexpr (std::is_pointer_v<T> || is_smart_pointer_v<T>)
				{
					return (*data) <= (*other);
				}
				else
				{
					return data <= other;
				}
			}

			bool operator!=(const T& other) const
			{
				if constexpr (std::is_pointer_v<T> || is_smart_pointer_v<T>)
				{
					return (*data) != (*other);
				}
				else
				{
					return data != other;
				}
			}
		};

		class Iterator
		{
		public:
			Iterator(Node* current) : m_current(current) {}

			bool operator!=(const Iterator& other) const { return m_current != other.m_current; }
			const T& operator*() const { return m_current->data; }
			const Iterator& operator++()
			{
				m_current = m_current->next;
				return *this;
			}
		private:
			Node* m_current;
		};
	};
}
