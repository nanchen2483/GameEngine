#include "enginepch.h"
#include "BufferLayoutUtil.h"

namespace Engine
{
	void UniformBufferStd140Util::AlignBufferElements(std::vector<BufferElement>& elements)
	{
		CleanBuffer();
		for (BufferElement& element : elements)
		{
			PadBufferElement(&element);
		}

		RecalculateComponentSizeInBuffer();
	}

	void UniformBufferStd140Util::PadBufferElement(BufferElement* element)
	{
		ENGINE_CORE_ASSERT(element->type != ShaderDataType::Mat3, "Invalid type in uniform buffer");
		if (element->size % TotalSizeOfComponents == 0)
		{
			// Re-calculate component size in the current buffer
			return RecalculateComponentSizeInBuffer();
		}

		if (element->size < ComponentSize)
		{
			// Update to the minimum component size
			element->size = ComponentSize;
		}

		if (!m_currentBuffer.empty() && element->size % 3 == 0)
		{
			// 3 is a magic number in the uniform buffer
			// Re-calculate component size in the current buffer
			RecalculateComponentSizeInBuffer();
		}

		if (m_currentBuffer.size() > 1)
		{
			const BufferElement* lastElement = m_currentBuffer.back();
			if (lastElement->size > element->size)
			{
				RecalculateComponentSizeInBuffer();
				AddToBuffer(element);
				return;
			}
		}

		uint32_t totalComponentSize = m_currentComponentSizeInBuffer + element->size;
		if (totalComponentSize == TotalSizeOfComponents)
		{
			// Keep the original size
			return CleanBuffer();
		}
		else if (totalComponentSize > TotalSizeOfComponents)
		{
			RecalculateComponentSizeInBuffer();
			AddToBuffer(element);
		}
		else
		{
			AddToBuffer(element);
		}
	}

	void UniformBufferStd140Util::RecalculateComponentSizeInBuffer()
	{
		switch (m_currentBuffer.size())
		{
		case 0:
			return;
		case 1:
		{
			m_currentBuffer.front()->size = TotalSizeOfComponents;
			break;
		}
		case 2:
		{
			BufferElement* firstElement = m_currentBuffer.front();
			BufferElement* secondElement = m_currentBuffer.back();
			if (firstElement->size == ComponentSize)
			{
				if (secondElement->size == ComponentSize)
				{
					secondElement->size = TotalSizeOfComponents - firstElement->size;
				}
				else if (secondElement->size == ComponentSize * 2)
				{
					firstElement->size = ComponentSize * 2;
				}
			}
			else if (firstElement->size == ComponentSize * 2)
			{
				secondElement->size = ComponentSize * 2;
			}
			break;
		}
		case 3:
		{
			m_currentBuffer.back()->size = ComponentSize * 2;
			break;
		}
		default:
			break;
		}

		return CleanBuffer();
	}

	void UniformBufferStd140Util::AddToBuffer(BufferElement* element)
	{
		m_currentBuffer.push_back(element);
		m_currentComponentSizeInBuffer += element->size;
	}

	void UniformBufferStd140Util::CleanBuffer()
	{
		m_currentBuffer.clear();
		m_currentComponentSizeInBuffer = 0;
	}
}

namespace Engine
{
	void BufferLayoutUtil::AlignBufferElements(BufferLayoutType type, std::vector<BufferElement>& elements)
	{
		switch (type)
		{
		case BufferLayoutType::Default:
			break;
		case BufferLayoutType::Std140:
		{
			UniformBufferStd140Util uniformBuffer;
			uniformBuffer.AlignBufferElements(elements);
			break;
		}
		case BufferLayoutType::Shared:
		case BufferLayoutType::Packed:
		case BufferLayoutType::Std430:
		default:
			ENGINE_CORE_ASSERT(false, "Invalid BufferLayoutType!");
			break;
		}
	}
}
