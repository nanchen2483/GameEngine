#pragma once
#include "AssimpAnimation.h"
#include "Engine/Renderer/Model/Model.h"
#include "Helper/AssimpHelper.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

namespace Engine
{
	class AssimpModel: public Model
	{
	public:
		AssimpModel(std::string const& path, bool gamma = false);
		AssimpModel(std::string const& path, bool gamma, int entityId);
		AssimpModel(std::string const& path, bool gamma, int entityId, Ptr<float> progression);
		
		virtual std::filesystem::path GetFilePath() override { return m_filePath; }
		virtual std::vector<Ptr<Mesh>> GetMeshes() const override { return m_meshes; }
		virtual std::vector<Ptr<Animation>> GetAnimations() const override { return m_animations; }
		virtual Ptr<BoundingBox> GetBoundingBox() const override { return m_boundingBox; }
		
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

		std::vector<Ptr<Mesh>> m_meshes;

		// Bounding volume
		Ptr<BoundingBox> m_boundingBox;
		glm::vec3 m_minAABB = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 m_maxAABB = glm::vec3(std::numeric_limits<float>::min());

		// Animations
		Dictionary<std::string, glm::mat4> m_boneOffsets;
		std::vector<Ptr<Animation>> m_animations;

		// Editor-only
		bool m_isLoaded = false;
		const int m_entityId;
		Ptr<float> m_progression = nullptr;
		float m_currentProgression = 0.0f;
		float m_totalProgression;
		void IncreaseProgression();
	};
}
