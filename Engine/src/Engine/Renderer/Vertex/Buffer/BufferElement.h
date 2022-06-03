#pragma once
#include "Engine/Core/Base.h"

namespace Engine
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	enum class BufferLayoutType
	{
		Default = 0,
		Shared,
		Packed,
		Std140,
		Std430
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Engine::ShaderDataType::Float:		return 4;
		case Engine::ShaderDataType::Float2:	return 4 * 2;
		case Engine::ShaderDataType::Float3:	return 4 * 3;
		case Engine::ShaderDataType::Float4:	return 4 * 4;
		case Engine::ShaderDataType::Mat3:		return 4 * 3 * 3;
		case Engine::ShaderDataType::Mat4:		return 4 * 4 * 4;
		case Engine::ShaderDataType::Int:		return 4;
		case Engine::ShaderDataType::Int2:		return 4 * 2;
		case Engine::ShaderDataType::Int3:		return 4 * 3;
		case Engine::ShaderDataType::Int4:		return 4 * 4;
		case Engine::ShaderDataType::Bool:		return 1;
		}

		ENGINE_CORE_ASSERT(false, "Uknow ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, bool normalized = false)
			: type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (type)
			{
			case Engine::ShaderDataType::Float:		return 1;
			case Engine::ShaderDataType::Float2:	return 2;
			case Engine::ShaderDataType::Float3:	return 3;
			case Engine::ShaderDataType::Float4:	return 4;
			case Engine::ShaderDataType::Mat3:		return 3 * 3;
			case Engine::ShaderDataType::Mat4:		return 4 * 4;
			case Engine::ShaderDataType::Int:		return 1;
			case Engine::ShaderDataType::Int2:		return 2;
			case Engine::ShaderDataType::Int3:		return 3;
			case Engine::ShaderDataType::Int4:		return 4;
			case Engine::ShaderDataType::Bool:		return 1;
			}

			ENGINE_CORE_ASSERT(false, "Unknow ShaderDataType!");
			return 0;
		}
	};
}
