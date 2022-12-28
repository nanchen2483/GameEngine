#include "enginepch.h"
#include "File.h"

#include "FileExtension.h"
#include "FileType.h"

#include <algorithm>
#include <set>

namespace Engine
{
	std::string File::GetFileType(std::filesystem::path file)
	{
		std::string extension = file.extension().string();

		std::set<std::string> scene{ FileExtension::SceneFile::Engine };
		if (std::find(scene.begin(), scene.end(), extension) != scene.end())
		{
			return FileType::Scene;
		}

		std::set<std::string> textures{ FileExtension::TextureFile::Png, FileExtension::TextureFile::Jpg, FileExtension::TextureFile::Bmp };
		if (std::find(textures.begin(), textures.end(), extension) != textures.end())
		{
			return FileType::Texture;
		}

		std::set<std::string> models{ FileExtension::ModelFile::Dae, FileExtension::ModelFile::Obj };
		if (std::find(models.begin(), models.end(), extension) != models.end())
		{
			return FileType::Model;
		}

		return "INVALID_FILE";
	}
}
