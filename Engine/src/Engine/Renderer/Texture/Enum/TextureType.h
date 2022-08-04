#pragma once

namespace Engine
{
	enum class TextureType
	{
		None = 0,
		Diffuse,
		Specular,
		Normal,
		Height,
		Skybox
	};

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

	enum class TextureAccessType
	{
		Unknown = -1,
		ReadOnly,
		WriteOnly,
		ReadWrite,
	};

	enum class TextureFormatType
	{
		Unknown = -1,
		RED,
		R16,
		R32,
		RG8,
		RG16,
		RG32,
		RGB8,
		RGB16,
		RGB32,
		RGBA8,
		RGBA16,
		RGBA32,
	};
}
