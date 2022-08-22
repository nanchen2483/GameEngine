#pragma once
#include "AssimpAnimation.h"
#include "AssimpMesh.h"
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
		AssimpModel(std::string const& path, bool gamma, int entityId, Ptr<TextureMap> textureMap);
		AssimpModel(std::string const& path, bool gamma, int entityId, Ptr<TextureMap> textureMap, Ptr<float> progression);
		
		virtual std::filesystem::path GetFilePath() override { return m_filePath; }
		virtual bool HasAnimations() override { return m_hasAnimations; }
		virtual std::vector<glm::mat4> GetBoneTransforms() const override;
		virtual BoundingValue GetBoundingValue() const override;
		virtual bool IsOnFrustum(const Frustum& frustum, const Transform& transform) const override;
		virtual void OnUpdate(float deltaTime) override;

		virtual const std::vector<AnimationInfo> GetAnimations() const override { return m_animationInfo; };
		virtual const AnimationInfo GetSelectedAnimation() const override { return m_selectedAnimationInfo; }
		virtual void SetSelectedAnimation(const AnimationInfo animation) override { m_selectedAnimationInfo = animation; }
		
		virtual void Draw() override;
	private:
		void Load(std::string const& path);
		void LoadMeshes(const aiScene* scene);
		void LoadAnimations(const aiScene* scene);
		void LoadBones(std::vector<Vertex>& vertices, const aiMesh* mesh);
		const Ptr<Material::MaterialTexture> LoadTexture(const aiMaterial* material, const aiTextureType type, const TextureType textureType);
		void UpdateBoundingValues(glm::vec3 position);

		const std::filesystem::path m_filePath;
		const std::filesystem::path m_directory = m_filePath.parent_path();
		const bool m_gammaCorrection;

		Ptr<TextureMap> m_textureMap;
		std::vector<AssimpMesh> m_meshes;

		// Bounding volume
		Uniq<BoundingVolume> m_boundingVolume;
		glm::vec3 m_minAABB = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 m_maxAABB = glm::vec3(std::numeric_limits<float>::min());

		// Animations
		bool m_hasAnimations = false;
		Dictionary<std::string, glm::mat4> m_boneOffsets;
		std::vector<AssimpAnimation> m_animations;
		std::vector<AnimationInfo> m_animationInfo;
		AnimationInfo m_selectedAnimationInfo;

		// Editor-only
		const int m_entityId;
		Ptr<float> m_progression = nullptr;
		float m_currentProgression = 0.0f;
		float m_totalProgression;
		void IncreaseProgression();
	};
}