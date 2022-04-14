#pragma once

#include <glm/glm.hpp>

#include "Engine/Renderer/Texture/Texture.h"

#define DIFFUSE_TEXTURE_SLOT 0
#define SPECULAR_TEXTURE_SLOT 1
#define NORMAL_TEXTURE_SLOT 2
#define HEIGHT_TEXTURE_SLOT 3

namespace Engine
{
	struct AssimpMaterial
	{
		Ptr<Texture> diffuse = nullptr;
		Ptr<Texture> specular = nullptr;
		Ptr<Texture> normal = nullptr;
		Ptr<Texture> height = nullptr;
		float shininess = 64.0f;

		void Bind()
		{
			if (diffuse != nullptr)
			{
				diffuse->Bind(DIFFUSE_TEXTURE_SLOT);
			}

			if (specular != nullptr)
			{
				specular->Bind(SPECULAR_TEXTURE_SLOT);
			}

			if (normal != nullptr)
			{
				normal->Bind(NORMAL_TEXTURE_SLOT);
			}

			if (height != nullptr)
			{
				height->Bind(HEIGHT_TEXTURE_SLOT);
			}
		}

		glm::vec3 GetIndex()
		{
			glm::vec3 index = glm::vec3(-1, -1, shininess);
			if (diffuse != nullptr)
			{
				index.x = DIFFUSE_TEXTURE_SLOT;
			}

			if (specular != nullptr)
			{
				index.y = SPECULAR_TEXTURE_SLOT;
			}

			return index;
		}
	};
}
