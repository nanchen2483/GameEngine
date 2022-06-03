#pragma once
#include "Engine/Renderer/Texture/Texture.h"

#include <glm/glm.hpp>

#define DIFFUSE_TEXTURE_SLOT 0
#define SPECULAR_TEXTURE_SLOT 1
#define NORMAL_TEXTURE_SLOT 2
#define HEIGHT_TEXTURE_SLOT 3

namespace Engine
{
	struct Material
	{
	public:
		struct MaterialTexture
		{
			Ptr<Image> image = nullptr;
			Ptr<Texture2D> texture = nullptr; // If already exists
		};

		Ptr<MaterialTexture> diffuse = nullptr;
		Ptr<MaterialTexture> specular = nullptr;
		Ptr<MaterialTexture> normal = nullptr;
		Ptr<MaterialTexture> height = nullptr;

		glm::vec3 GetTextureSlotIndices()
		{
			int diffuseIndex = (diffuse == nullptr) ? -1 : DIFFUSE_TEXTURE_SLOT;
			int specularIndex = (specular == nullptr) ? -1 : SPECULAR_TEXTURE_SLOT;
			int normalIndex = (normal == nullptr) ? -1 : NORMAL_TEXTURE_SLOT;
			int heightIndex = (height == nullptr) ? -1 : HEIGHT_TEXTURE_SLOT;
			return glm::vec3(diffuseIndex, specularIndex, shininess);
		}

		void Setup(Ptr<TextureMap> textureMap)
		{
			if (diffuse != nullptr && diffuse->image != nullptr)
			{
				diffuse->texture = Texture2D::Create(diffuse->image, TextureType::Diffuse);
				(*textureMap)[diffuse->texture->GetFilePath()] = diffuse->texture;
				diffuse->image = nullptr;
			}

			if (specular != nullptr && specular->image != nullptr)
			{
				specular->texture = Texture2D::Create(specular->image, TextureType::Specular);
				(*textureMap)[specular->texture->GetFilePath()] = specular->texture;
				specular->image = nullptr;
			}

			if (normal != nullptr && normal->image != nullptr)
			{
				normal->texture = Texture2D::Create(normal->image, TextureType::Normal);
				(*textureMap)[normal->texture->GetFilePath()] = normal->texture;
				normal->image = nullptr;
			}

			if (height != nullptr && height->image != nullptr)
			{
				height->texture = Texture2D::Create(height->image, TextureType::Height);
				(*textureMap)[height->texture->GetFilePath()] = height->texture;
				height->image = nullptr;
			}
		}
		void Bind()
		{
			if (diffuse != nullptr)
			{
				diffuse->texture->Bind(DIFFUSE_TEXTURE_SLOT);
			}

			if (specular != nullptr)
			{
				specular->texture->Bind(SPECULAR_TEXTURE_SLOT);
			}

			if (normal != nullptr)
			{
				normal->texture->Bind(NORMAL_TEXTURE_SLOT);
			}

			if (height != nullptr)
			{
				height->texture->Bind(HEIGHT_TEXTURE_SLOT);
			}
		}
	private:
		float shininess = 64.0f;
	};
}
