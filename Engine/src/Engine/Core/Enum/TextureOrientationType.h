#pragma once
#include <string>

#include <Engine/Core/Log.h>

namespace Engine
{
	enum class TextureOrientationType
	{
		None = -1,
		Right,
		Left,
		Top,
		Bottom,
		Back,
		Front,
	};

	namespace Enum
	{
		static const char* ToString(TextureOrientationType o)
		{
			switch (o)
			{
			case TextureOrientationType::None:
				return nullptr;
			case TextureOrientationType::Right:
				return "Right";
			case TextureOrientationType::Left:
				return "Left";
			case TextureOrientationType::Top:
				return "Top";
			case TextureOrientationType::Bottom:
				return "Bottom";
			case TextureOrientationType::Back:
				return "Back";
			case TextureOrientationType::Front:
				return "Front";
			default:
				ENGINE_CORE_ERROR("Unsupported Orientation type");
				break;
			}
		}
	}
}
