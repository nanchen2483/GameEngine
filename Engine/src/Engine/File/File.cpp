#include "enginepch.h"
#include "File.h"

#include "FileExtension.h"
#include "FileType.h"

namespace Engine
{
	std::string File::GetFileType(std::filesystem::path file)
	{
		std::string extension = file.extension().string();

		std::set<std::string> scenes = FileExtension::SceneExtension::All;
		if (std::find(scenes.begin(), scenes.end(), extension) != scenes.end())
		{
			return FileType::Scene;
		}

		std::set<std::string> textures = FileExtension::TextureExtension::All;
		if (std::find(textures.begin(), textures.end(), extension) != textures.end())
		{
			return FileType::Texture;
		}

		std::set<std::string> models = FileExtension::ModelExtension::All;
		if (std::find(models.begin(), models.end(), extension) != models.end())
		{
			return FileType::Model;
		}

		return "INVALID_FILE";
	}
}
