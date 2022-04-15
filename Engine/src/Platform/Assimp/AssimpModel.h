#pragma once

#include "Engine/Renderer/Model/Model.h"
#include "AssimpAnimation.h"
#include "AssimpMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <filesystem>

namespace Engine
{
	class AssimpModel: public Model
	{
	public:
		AssimpModel(std::string const& path);
		AssimpModel(std::string const& path, bool gamma);
		AssimpModel(std::string const& path, bool gamma, int entityId, TextureMap* textureMap);
		virtual std::string GetFilePath() override { return m_filePath.string(); }
		
		virtual bool HasAnimations() override { return m_animation != nullptr; }
		virtual std::vector<glm::mat4> GetBoneTransforms(float deltaTime) override;
		virtual float* GetAnimationTime() override;
		virtual const float GetAnimationDuration() override;
		
		virtual void Draw() override;
	private:
		void Load(std::string const& path);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		AssimpMesh CreateMesh(const aiMesh* mesh, const aiMaterial* material);
		AssimpMaterial LoadMaterial(const aiMaterial* material);
		Ptr<Texture> LoadTexture(const aiMaterial* material, const aiTextureType type, const TextureType textureType);
		void ExtractBoneWeight(std::vector<Vertex>& vertices, const aiMesh* mesh);
	private:
		const std::filesystem::path m_filePath;
		const std::filesystem::path m_directory = m_filePath.parent_path();
		const bool m_gammaCorrection;
		TextureMap* m_textureMap;
		std::vector<AssimpMesh> m_meshes;

		// Animation
		Uniq<AssimpAnimation> m_animation;

		// Editor-only
		const int m_entityId;
	};
}
