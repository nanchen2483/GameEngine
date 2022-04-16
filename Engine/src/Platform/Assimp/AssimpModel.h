#pragma once

#include "Engine/Renderer/Model/Model.h"
#include "AssimpAnimation.h"
#include "AssimpMesh.h"
#include "Helper/AssimpHelper.h"

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
		virtual bool HasAnimations() override { return m_hasAnimations; }
		virtual void OnUpdate(float deltaTime) override;
		virtual std::vector<glm::mat4> GetBoneTransforms() const override;
		
		virtual const std::vector<AnimationInfo> GetAnimations() const override { return m_animationInfo; };
		virtual const AnimationInfo GetSelectedAnimation() const override { return m_selectedAnimationInfo; }
		virtual void SetSelectedAnimation(const AnimationInfo animation) override { m_selectedAnimationInfo = animation; }
		
		virtual void Draw() override;
	private:
		void Load(std::string const& path);
		void SetupMeshes(const aiScene* scene);
		void SetupAnimations(const aiScene* scene);
		const AssimpMaterial LoadMaterial(const aiMaterial* material);
		const Ptr<Texture> LoadTexture(const aiMaterial* material, const aiTextureType type, const TextureType textureType);
		void ExtractBoneWeight(std::vector<Vertex>& vertices, const aiMesh* mesh);

		const std::filesystem::path m_filePath;
		const std::filesystem::path m_directory = m_filePath.parent_path();
		const bool m_gammaCorrection;

		TextureMap* m_textureMap;
		std::vector<AssimpMesh> m_meshes;

		// Animations
		bool m_hasAnimations = false;
		Dictionary<std::string, glm::mat4> m_boneOffsets;
		std::vector<AssimpAnimation> m_animations;
		std::vector<AnimationInfo> m_animationInfo;
		AnimationInfo m_selectedAnimationInfo;

		// Editor-only
		const int m_entityId;
	};
}
