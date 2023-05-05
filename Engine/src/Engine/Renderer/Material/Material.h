#pragma once
#include "Engine/Library/TextureLibrary.h"

#include <glm/glm.hpp>
#include <string>

#define DIFFUSE_TEXTURE_SLOT 0
#define SPECULAR_TEXTURE_SLOT 1
#define NORMAL_TEXTURE_SLOT 2
#define HEIGHT_TEXTURE_SLOT 3

namespace Engine
{
	struct Material
	{
		const float SHININESS = 64.0f;

		std::string diffusePath{};
		std::string specularPath{};
		std::string normalPath{};
		std::string heightPath{};

		glm::vec3 GetTextureSlotIndices()
		{
			int diffuseIndex = (diffusePath.empty()) ? -1 : DIFFUSE_TEXTURE_SLOT;
			int specularIndex = (specularPath.empty()) ? -1 : SPECULAR_TEXTURE_SLOT;
			int normalIndex = (normalPath.empty()) ? -1 : NORMAL_TEXTURE_SLOT;
			int heightIndex = (heightPath.empty()) ? -1 : HEIGHT_TEXTURE_SLOT;
			return glm::vec3(diffuseIndex, specularIndex, SHININESS);
		}

		void Bind()
		{
			if (!diffusePath.empty())
			{
				TextureLibrary::Load(diffusePath, TextureType::Diffuse, true)->Bind(DIFFUSE_TEXTURE_SLOT);
			}

			if (!specularPath.empty())
			{
				TextureLibrary::Load(specularPath, TextureType::Specular, true)->Bind(SPECULAR_TEXTURE_SLOT);
			}

			if (!normalPath.empty())
			{
				TextureLibrary::Load(normalPath, TextureType::Normal, true)->Bind(NORMAL_TEXTURE_SLOT);
			}

			if (!heightPath.empty())
			{
				TextureLibrary::Load(heightPath, TextureType::Height, true)->Bind(HEIGHT_TEXTURE_SLOT);
			}
		}
	};
}
