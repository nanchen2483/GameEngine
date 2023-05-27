#pragma once
#include "Engine/Renderer/Buffer/BufferElement.h"

#include <vector>

namespace Engine
{
	class UniformBufferStd140Util
	{
	public:
		void AlignBufferElements(std::vector<BufferElement>& elements);
	private:
		void PadBufferElement(BufferElement* element);
		void RecalculateComponentSizeInBuffer();
		void AddToBuffer(BufferElement* element);
		void CleanBuffer();
		std::vector<BufferElement*> m_currentBuffer;
		uint32_t m_currentComponentSizeInBuffer;
		
		const uint32_t ComponentCount = 4;
		const uint32_t ComponentSize = 4;
		const uint32_t TotalSizeOfComponents = ComponentCount * ComponentSize;
	};

	class BufferLayoutUtil
	{
	public:
		static void AlignBufferElements(BufferLayoutType type, std::vector<BufferElement>& elements);
	};
}
