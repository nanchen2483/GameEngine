#pragma once
#include "AssimpAnimation.h"
#include "Engine/Renderer/Model/IModel.h"
#include "Helper/AssimpHelper.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

namespace Engine
{
	class AssimpModel: public IModel
	{
	public:
		AssimpModel(std::string const& path);
		AssimpModel(std::string const& path, bool gamma);
		AssimpModel(std::string const& path, bool gamma, Ptr<float> progression);
		
		virtual std::filesystem::path GetFilePath() override { return m_filePath; }
		virtual std::vector<Ptr<IMesh>> GetMeshes() const override { return m_meshes; }
		virtual std::vector<Ptr<IAnimation>> GetAnimations() const override { return m_animations; }
		virtual Ptr<IBoundingBox> GenerateBoundingBox() override;
		
		virtual Uid GetUid() const override { return m_uid; }
	private:
		void Load(std::string const& path);
		void LoadMeshes(const aiScene* scene);
		void LoadAnimations(const aiScene* scene);
		void LoadBones(std::vector<Vertex>& vertices, const aiMesh* mesh);
		std::string GetTexturePath(const aiMaterial* material, const aiTextureType type, const TextureType textureType);
		void UpdateBoundingValues(glm::vec3 position);

		Uid m_uid;
		const std::filesystem::path m_filePath;
		const std::filesystem::path m_directory = m_filePath.parent_path();
		const bool m_gammaCorrection;

		std::vector<Ptr<IMesh>> m_meshes;

		// Bounding volume
		glm::vec3 m_minAABB = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 m_maxAABB = glm::vec3(std::numeric_limits<float>::min());

		// Animations
		Dictionary<std::string, glm::mat4> m_boneOffsets;
		std::vector<Ptr<IAnimation>> m_animations;

		// Editor-only
		bool m_isLoaded = false;
		Ptr<float> m_progression = nullptr;
		float m_currentProgression = 0.0f;
		float m_totalProgression;
		void IncreaseProgression();
	};
}
