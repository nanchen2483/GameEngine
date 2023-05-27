#include <gtest/gtest.h>
#include "Engine.h"
#include "Engine/Renderer/Buffer/BufferLayout.h"

namespace EngineTest
{
	struct BufferElementResult
	{
		Engine::ShaderDataType type;
		uint32_t componentCount;
		uint32_t offset;
		uint32_t size;

		BufferElementResult(
			Engine::ShaderDataType type,
			uint32_t componentCount,
			uint32_t offset,
			uint32_t size)
			: type(type)
			, componentCount(componentCount)
			, offset(offset)
			, size(size)
		{
		}
	};

	struct UniformBufferLayoutTest : testing::Test
	{
		UniformBufferLayoutTest() = default;
	};

	TEST_F(UniformBufferLayoutTest, SimpleLayout)
	{
		// Arrange
		std::vector<BufferElementResult> expected
		{
			{ Engine::ShaderDataType::Float3, 3,   0, 12 },
			{ Engine::ShaderDataType::Float,  1,  12,  4 },
			{ Engine::ShaderDataType::Float2, 2,  16,  8 },
			{ Engine::ShaderDataType::Float,  1,  24,  4 },
			{ Engine::ShaderDataType::Float,  1,  28,  4 },
			{ Engine::ShaderDataType::Float3, 3,  32, 12 },
			{ Engine::ShaderDataType::Float,  1,  44,  4 },
			{ Engine::ShaderDataType::Mat4,  16,  48, 64 },
		};

		// Act
		Engine::BufferLayout layout = Engine::BufferLayout(
			{
				Engine::BufferLayoutType::Std140,
				{
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float2 },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Mat4 },
				},
			});

		// Assert
		EXPECT_EQ(expected.size(), layout.GetNumOfElements());

		for (uint32_t i = 0; i < expected.size(); ++i)
		{
			EXPECT_EQ(expected[i].type, layout[i].type);
			EXPECT_EQ(expected[i].componentCount, layout[i].GetComponentCount());
			EXPECT_EQ(expected[i].offset, layout[i].offset);
			EXPECT_EQ(expected[i].size, layout[i].size);
		}
	}

	TEST_F(UniformBufferLayoutTest, ComplexLayout)
	{
		// Arrange
		std::vector<BufferElementResult> expected
		{
			{ Engine::ShaderDataType::Float3, 3,   0, 12 },
			{ Engine::ShaderDataType::Int,	  1,  12,  4 },
			{ Engine::ShaderDataType::Int,    1,  16, 16 },
			{ Engine::ShaderDataType::Float3, 3,  32, 12 },
			{ Engine::ShaderDataType::Float,  1,  44,  4 },
			{ Engine::ShaderDataType::Float2, 2,  48, 16 },
			{ Engine::ShaderDataType::Mat4,  16,  64, 64 },
			{ Engine::ShaderDataType::Bool,   1, 128,  8 },
			{ Engine::ShaderDataType::Int2,   2, 136,  8 },
			{ Engine::ShaderDataType::Int3,   3, 144, 16 },
		};

		// Act
		Engine::BufferLayout layout = Engine::BufferLayout(
			{
				Engine::BufferLayoutType::Std140,
				{
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Int },
					{ Engine::ShaderDataType::Int },
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float2 },
					{ Engine::ShaderDataType::Mat4 },
					{ Engine::ShaderDataType::Bool },
					{ Engine::ShaderDataType::Int2 },
					{ Engine::ShaderDataType::Int3 },
				},
			});

		// Assert
		EXPECT_EQ(expected.size(), layout.GetNumOfElements());

		for (uint32_t i = 0; i < expected.size(); ++i)
		{
			EXPECT_EQ(expected[i].type, layout[i].type);
			EXPECT_EQ(expected[i].componentCount, layout[i].GetComponentCount());
			EXPECT_EQ(expected[i].offset, layout[i].offset);
			EXPECT_EQ(expected[i].size, layout[i].size);
		}
	}

	TEST_F(UniformBufferLayoutTest, ComplexFloatLayout)
	{
		// Arrange
		std::vector<BufferElementResult> expected
		{
			{ Engine::ShaderDataType::Float3, 3,  0, 12 },
			{ Engine::ShaderDataType::Float,  1, 12,  4 },
			{ Engine::ShaderDataType::Float,  1, 16,  4 },
			{ Engine::ShaderDataType::Float,  1, 20, 12 },
			{ Engine::ShaderDataType::Float3, 3, 32, 16 },
			{ Engine::ShaderDataType::Float3, 3, 48, 16 },
			{ Engine::ShaderDataType::Float3, 3, 64, 16 },
		};

		// Act
		Engine::BufferLayout layout = Engine::BufferLayout(
			{
				Engine::BufferLayoutType::Std140,
				{
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float },
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Float3 },
					{ Engine::ShaderDataType::Float3 },
				},
			});

		// Assert
		EXPECT_EQ(expected.size(), layout.GetNumOfElements());

		for (uint32_t i = 0; i < expected.size(); ++i)
		{
			EXPECT_EQ(expected[i].type, layout[i].type);
			EXPECT_EQ(expected[i].componentCount, layout[i].GetComponentCount());
			EXPECT_EQ(expected[i].offset, layout[i].offset);
			EXPECT_EQ(expected[i].size, layout[i].size);
		}
	}

	TEST_F(UniformBufferLayoutTest, ComplexIntLayout)
	{
		// Arrange
		std::vector<BufferElementResult> expected
		{
			{ Engine::ShaderDataType::Int3, 3,  0, 12 },
			{ Engine::ShaderDataType::Int,  1, 12,  4 },
			{ Engine::ShaderDataType::Int,  1, 16,  8 },
			{ Engine::ShaderDataType::Int2, 2, 24,  8 },
			{ Engine::ShaderDataType::Int3, 3, 32, 16 },
			{ Engine::ShaderDataType::Int4, 4, 48, 16 },
			{ Engine::ShaderDataType::Int3, 3, 64, 16 },
		};

		// Act
		Engine::BufferLayout layout = Engine::BufferLayout(
			{
				Engine::BufferLayoutType::Std140,
				{
					{ Engine::ShaderDataType::Int3 },
					{ Engine::ShaderDataType::Int },
					{ Engine::ShaderDataType::Int },
					{ Engine::ShaderDataType::Int2 },
					{ Engine::ShaderDataType::Int3 },
					{ Engine::ShaderDataType::Int4 },
					{ Engine::ShaderDataType::Int3 },
				},
			});

		// Assert
		EXPECT_EQ(expected.size(), layout.GetNumOfElements());

		for (uint32_t i = 0; i < expected.size(); ++i)
		{
			EXPECT_EQ(expected[i].type, layout[i].type);
			EXPECT_EQ(expected[i].componentCount, layout[i].GetComponentCount());
			EXPECT_EQ(expected[i].offset, layout[i].offset);
			EXPECT_EQ(expected[i].size, layout[i].size);
		}
	}
}
