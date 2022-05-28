#pragma once

#include "BufferElement.h"
#include "Engine/Util/BufferLayoutUtil.h"

namespace Engine
{
	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: BufferLayout(BufferLayoutType::Default, elements)
		{
		}

		BufferLayout(const BufferLayoutType type, const std::initializer_list<BufferElement>& elements)
			: m_type(type), m_elements(elements)
		{
			BufferLayoutUtil::RecalculateSize(m_type, m_elements);
			CalculateOffset();
		}

		inline uint32_t GetStride() const { return m_stride; }
		inline uint32_t GetNumOfElements() const { return m_elements.size(); }
		inline const std::vector<BufferElement> GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

		const BufferElement& operator[] (const uint32_t index) { return m_elements[index]; }
	private:
		void CalculateOffset()
		{
			m_stride = 0;
			for (BufferElement& element : m_elements)
			{
				element.offset = m_stride;
				m_stride += element.size;
			}
		}
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
		BufferLayoutType m_type;
	};
}
