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
		
		virtual bool HasAnimations() override { return m_hasAnimation; }
		virtual std::vector<glm::mat4> GetBoneTransforms(float deltaTime) override;
		virtual float* GetAnimationTime() override;
		virtual const float GetAnimationDuration() override;
		
		virtual void Draw() override;
	private:
		void Load(std::string const& path);
		void SetupMeshes(const aiNode* node, const aiScene* scene);
		void SetupAnimations(const aiScene* scene);
		const AssimpMesh CreateMesh(const aiMesh* mesh, const aiMaterial* material);
		const AssimpMaterial LoadMaterial(const aiMaterial* material);
		const Ptr<Texture> LoadTexture(const aiMaterial* material, const aiTextureType type, const TextureType textureType);
		void ExtractBoneWeight(std::vector<Vertex>& vertices, const aiMesh* mesh);

		const std::filesystem::path m_filePath;
		const std::filesystem::path m_directory = m_filePath.parent_path();
		const bool m_gammaCorrection;
		bool m_hasAnimation = false;

		TextureMap* m_textureMap;
		std::vector<AssimpMesh> m_meshes;

		// Animations
		Dictionary<std::string, glm::mat4> m_boneOffsetDict;
		std::vector<AssimpAnimation> m_animations;
		uint32_t m_selectedAnimation = 0;

		// Editor-only
		const int m_entityId;
	};
}
