#pragma once
#include <cstdint>

namespace Engine
{
	struct LightBoxData
	{
		static const uint32_t numOfVertices = 24;
		static const uint32_t numOfIndices = 36;
		static constexpr float vertices[numOfVertices]
		{
			// front
			-0.5, -0.5,  0.5,
			 0.5, -0.5,  0.5,
			 0.5,  0.5,  0.5,
			-0.5,  0.5,  0.5,
			// back
			-0.5, -0.5, -0.5,
			 0.5, -0.5, -0.5,
			 0.5,  0.5, -0.5,
			-0.5,  0.5, -0.5
		};

		static constexpr uint8_t indices[numOfIndices]
		{
			// front
			0, 1, 2,
			2, 3, 0,
			// right
			1, 5, 6,
			6, 2, 1,
			// back
			7, 6, 5,
			5, 4, 7,
			// left
			4, 0, 3,
			3, 7, 4,
			// bottom
			4, 5, 1,
			1, 0, 4,
			// top
			3, 2, 6,
			6, 7, 3
		};
	};
}
