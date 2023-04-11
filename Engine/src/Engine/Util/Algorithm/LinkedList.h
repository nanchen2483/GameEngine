#pragma once
#include "Engine/Core/System/Object/IComparable.h"

#include <vector>

namespace Engine
{
	template<typename T>
	class LinkedList
	{
	private:
		struct Node;
		class Iterator;
	public:
		LinkedList() : m_head(nullptr), m_tail(nullptr) {}
		~LinkedList()
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

		T& GetHeadValue() const { return m_head->value; }
		T& GetTailValue() const { return m_tail->value; }

		void Add(const T& value)
		{
			Node* newNode = new Node(value);
			if (m_head == nullptr)
			{
				m_head = newNode;
				m_tail = newNode;
			}
			else
			{
				Node* current = m_head;
				while (current != nullptr && *(current->value) < *value)
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

		void Delete(const T& value, bool lazy = false)
		{
			Node* current = m_head;
			while (current != nullptr && current->value != value)
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

		Iterator begin() { return Iterator(m_head); }
		Iterator end() { return Iterator(nullptr); }
	private:
		Node* m_head;
		Node* m_tail;
		std::vector<Node*> m_deleted;

		struct Node
		{
			T value;
			Node* prev;
			Node* next;

			Node(const T& value) : value(value), next(nullptr), prev(nullptr) {}
			~Node()
			{
				if constexpr (std::is_pointer_v<T>)
				{
					delete value;
				}
			}
		};

		class Iterator
		{
		public:
			Iterator(Node* start) : m_current(start) {}

			bool operator != (const Iterator& other) const { return m_current != other.m_current; }
			T& operator*() { return m_current->value; }
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
