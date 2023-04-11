#pragma once
#include "Engine/Core/System/Object/IComparable.h"

#include <type_traits>
#include <vector>

namespace Engine
{
	template<typename T, typename Enable = std::enable_if_t<std::is_base_of_v<IComparable<T>, T>>>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();

		const T& GetHeadValue() const;
		const T& GetTailValue() const;

		void Add(const T& data);
		void Delete(const T& data);
		void Delete(const T& data, bool lazy);

		class Iterator;
		Iterator begin() { return Iterator(m_head); }
		Iterator end() { return Iterator(nullptr); }
	private:
		struct Node;
		Node* m_head;
		Node* m_tail;
		std::vector<Node*> m_deleted;

		struct Node
		{
			T data;
			Node* prev;
			Node* next;

			Node(T data);
			~Node();

			bool operator<(const T& other) const;
			bool operator!=(const T& other) const;
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


/// <summary>
/// LinkedList source code
/// </summary>
namespace Engine
{
	template<typename T, typename _>
	LinkedList<T, _>::LinkedList()
	{
		m_head = nullptr;
		m_tail = nullptr;
	}

	template<typename T, typename _>
	LinkedList<T, _>::~LinkedList()
	{
		Node* current = m_head;
		while (current != nullptr)
		{
			Node* next = current->next;
			delete current;
			current = next;
		}

		for (uint32_t i = 0; i < m_deleted.size(); ++i)
		{
			delete m_deleted[i];
		}
	}

	template<typename T, typename _>
	const T& LinkedList<T, _>::GetHeadValue() const
	{
		ENGINE_CORE_ASSERT(m_head != nullptr, "Linked list is empty.");
		return m_head->data;
	}

	template<typename T, typename _>
	const T& LinkedList<T, _>::GetTailValue() const
	{
		ENGINE_CORE_ASSERT(m_tail != nullptr, "Linked list is empty.");
		return m_tail->data;
	}

	template<typename T, typename _>
	void LinkedList<T, _>::Add(const T& data)
	{
		Node* newNode = new Node(data);
		if (m_head == nullptr)
		{
			m_head = newNode;
			m_tail = newNode;
		}
		else
		{
			Node* current = m_head;
			while (current != nullptr && *current < data)
			{
				current = current->next;
			}

			if (current == nullptr)
			{
				newNode->prev = m_tail;
				m_tail->next = newNode;
				m_tail = newNode;
			}
			else if (current == m_head)
			{
				newNode->next = m_head;
				m_head->prev = newNode;
				m_head = newNode;
			}
			else
			{
				newNode->next = current;
				newNode->prev = current->prev;
				current->prev->next = newNode;
				current->prev = newNode;
			}
		}
	}

	template<typename T, typename _>
	void LinkedList<T, _>::Delete(const T& data)
	{
		Delete(data, false);
	}

	template<typename T, typename _>
	void LinkedList<T, _>::Delete(const T& data, bool lazy)
	{
		Node* current = m_head;
		while (current != nullptr && *current != data)
		{
			current = current->next;
		}

		if (current == nullptr)
		{
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

		if (lazy)
		{
			m_deleted.push_back(current);
		}
		else
		{
			delete current;
		}
	}
}


/// <summary>
/// LinkedList Node source code
/// </summary>
namespace Engine
{
	template<typename T, typename _>
	LinkedList<T, _>::Node::Node(T data)
		: data(data)
	{
		prev = nullptr;
		next = nullptr;
	}

	template<typename T, typename _>
	LinkedList<T, _>::Node::~Node()
	{
		if constexpr (std::is_pointer_v<T>)
		{
			delete data;
		}
	}

	template<typename T, typename _>
	bool LinkedList<T, _>::Node::operator<(const T& other) const
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return (*data) < (*other);
		}
		else
		{
			return data < other;
		}
	}

	template<typename T, typename _>
	bool LinkedList<T, _>::Node::operator!=(const T& other) const
	{
		if constexpr (std::is_pointer_v<T>)
		{
			return (*data) != (*other);
		}
		else
		{
			return data != other;
		}
	}
}
