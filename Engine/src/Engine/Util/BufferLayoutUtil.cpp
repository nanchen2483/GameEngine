#include "enginepch.h"
#include "BufferLayoutUtil.h"

namespace Engine
{
	void UniformBufferStd140Util::RecalculateSize(std::vector<BufferElement>& elements)
	{
		Reset();
		for (BufferElement& element : elements)
		{
			AddComponent(&element);
		}
	}

	void UniformBufferStd140Util::AddComponent(BufferElement* element)
	{
		ENGINE_CORE_ASSERT(element->type != ShaderDataType::Mat3, "Invalid type in uniform buffer");
		if (element->size % TOTAL_SIZE_OF_COMPONENTS == 0)
		{
			return UpdateSize();
		}

		if (element->size < COMPONENT_SIZE)
		{
			element->size = COMPONENT_SIZE;
		}

		if (!m_currentBuffer.empty() && element->size % 3 == 0)
		{
			UpdateSize();
		}

		if (m_currentBuffer.size() > 1)
		{
			const BufferElement* lastElement = m_currentBuffer.back();
			if (lastElement->size > element->size)
			{
				UpdateSize();
				m_currentBufferSize = element->size;
				m_currentBuffer.push_back(element);
				return;
			}
		}

		uint32_t newTotalSize = m_currentBufferSize + element->size;
		if (newTotalSize == TOTAL_SIZE_OF_COMPONENTS)
		{
			// Keep the original size
			return Reset();
		}
		else if (newTotalSize > TOTAL_SIZE_OF_COMPONENTS)
		{
			UpdateSize();
			m_currentBufferSize = element->size;
			m_currentBuffer.push_back(element);
		}
		else if (newTotalSize < TOTAL_SIZE_OF_COMPONENTS)
		{
			m_currentBufferSize = newTotalSize;
			m_currentBuffer.push_back(element);
		}
	}

	void UniformBufferStd140Util::UpdateSize()
	{
		switch (m_currentBuffer.size())
		{
		case 0:
			return;
		case 1:
		{
			m_currentBuffer.front()->size = TOTAL_SIZE_OF_COMPONENTS;
			break;
		}
		case 2:
		{
			BufferElement* firstElement = m_currentBuffer.front();
			BufferElement* secondElement = m_currentBuffer.back();
			if (firstElement->size == COMPONENT_SIZE)
			{
				if (secondElement->size == COMPONENT_SIZE)
				{
					secondElement->size = TOTAL_SIZE_OF_COMPONENTS - firstElement->size;
				}
				else if (secondElement->size == COMPONENT_SIZE * 2)
				{
					firstElement->size = COMPONENT_SIZE * 2;
				}
			}
			else if (firstElement->size == COMPONENT_SIZE * 2)
			{
				secondElement->size = COMPONENT_SIZE * 2;
			}
			break;
		}
		case 3:
		{
			m_currentBuffer.back()->size = COMPONENT_SIZE * 2;
			break;
		}
		default:
			break;
		}

		return Reset();
	}

	void UniformBufferStd140Util::Reset()
	{
		m_currentBuffer.clear();
		m_currentBufferSize = 0;
	}
}

namespace Engine
{
	void BufferLayoutUtil::RecalculateSize(BufferLayoutType type, std::vector<BufferElement>& elements)
	{
		switch (type)
		{
		case BufferLayoutType::Default:
			break;
		case BufferLayoutType::Std140:
		{
			UniformBufferStd140Util uniformBuffer;
			uniformBuffer.RecalculateSize(elements);
			break;
		}
		case BufferLayoutType::Shared:
		case BufferLayoutType::Packed:
		case BufferLayoutType::Std430:
		default:
			ENGINE_CORE_ASSERT(false, "BufferLayoutType: {0} not supported!", type);
			break;
		}
	}
}
