#pragma once
#include <vector>

#include "Engine/Renderer/Vertex/Buffer/BufferElement.h"

namespace Engine
{
	class UniformBufferStd140Util
	{
	public:
		void RecalculateSize(std::vector<BufferElement>& elements);
	private:
		void AddComponent(BufferElement* element);
		void UpdateSize();
		void Reset();
	private:
		std::vector<BufferElement*> m_currentBuffer;
		uint32_t m_currentBufferSize;
		
		const uint32_t NUM_OF_COMPONENTS = 4;
		const uint32_t COMPONENT_SIZE = 4;
		const uint32_t TOTAL_SIZE_OF_COMPONENTS = NUM_OF_COMPONENTS * COMPONENT_SIZE;
	};

	class BufferLayoutUtil
	{
	public:
		static void RecalculateSize(BufferLayoutType type, std::vector<BufferElement>& elements);
	};
}
