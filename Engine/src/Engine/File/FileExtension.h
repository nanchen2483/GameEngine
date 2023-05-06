#pragma once
#include <string>
#include <set>

namespace Engine
{
	namespace FileExtension
	{
		namespace SceneExtension
		{
			const std::string Engine = ".engine";
			const std::set<std::string> All
			{
				Engine
			};
		};

		namespace TextureExtension
		{
			const std::string Png = ".png";
			const std::string Jpg = ".jpg";
			const std::string Bmp = ".bmp";
			const std::set<std::string> All
			{
				Png,
				Jpg,
				Bmp
			};
		};

		namespace ModelExtension
		{
			const std::string Dae = ".dae";
			const std::string Obj = ".obj";
			const std::string Blend = ".blend";
			const std::set<std::string> All
			{
				Dae,
				Obj,
				Blend
			};
		};
	}
}
