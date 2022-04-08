#pragma once

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

	enum class ShaderDataName
	{
		None = 0,
		Position,
		Normal,
		Color,
		TexCoord,
		Material,
		Tangent,
		Bitangent,
		Bone1,
		Bone2,
		BoneIds,
		Weights,
		EntityId
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
		ShaderDataName name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, ShaderDataName name, bool normalized = false)
			: name(name), type(type), size(ShaderDataTypeSize(type)), offset(0), normalized(normalized)
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

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements)
		{
			CalculateOffset();
		}

		inline uint32_t GetStride() const { return m_stride; }
		inline const std::vector<BufferElement> GetElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		void CalculateOffset()
		{
			uint32_t offset = 0;
			m_stride = 0;
			for (Engine::BufferElement& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride = 0;
	};
}
