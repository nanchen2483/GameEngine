#pragma once
#include <cstdint>

namespace Engine
{
	struct ShadowDebugData
	{
		static const uint32_t numOfVertices = 20;
		static const uint32_t numOfIndices = 6;
		static constexpr float vertices[numOfVertices]
		{
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		static constexpr uint8_t indices[numOfIndices]
		{
			0, 1, 2,
			2, 1, 3,
		};
	};
}
