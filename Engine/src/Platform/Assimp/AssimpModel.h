#pragma once

#include "Engine/Renderer/Model/Model.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "AssimpMesh.h"
#include "AssimpNode.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

namespace Engine
{
	class AssimpModel: public Model
	{
	public:
		AssimpModel(std::string const& path);
		AssimpModel(std::string const& path, bool gamma);
		AssimpModel(std::string const& path, bool gamma, int entityId, TextureMap* textureMap);
		virtual std::string& GetPath() override { return m_path; }
		virtual void UpdateAnimation(float deltaTime) override;
		virtual std::vector<glm::mat4> GetPoseTransforms() override { return m_transforms; }
		virtual bool IsAnimationModel() override { return m_includeAnimation; };
		virtual void Draw() override;
		
	private:
		void Load(std::string const& path);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		AssimpMesh ProcessMesh(const aiMesh* mesh, const aiMaterial* material);
		Ptr<Texture> LoadMaterialTexture(const aiMaterial* material, aiTextureType type, TextureType textureType);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh);
		void CalculateBoneTransform(const Ptr<AssimpNode>& node, glm::mat4 parentTransform);

	private:
		std::string m_path;
		std::string m_directory;
		bool m_gammaCorrection;
		std::vector<AssimpMesh> m_meshes;
		TextureMap* m_textureMap;

		// Animation
		bool m_includeAnimation = false;
		Ptr<AssimpAnimation> m_animation;
		Ptr<AssimpNode> m_rootNode;
		std::vector<glm::mat4> m_transforms;
		float m_animationTime = 0.0f;

		// Editor-only
		int m_entityId;
	};
}
