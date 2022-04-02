#pragma once

#include "IModel.h"
#include "Data/Animation.h"
#include "Data/Bone.h"
#include "Data/Mesh.h"
#include "Engine/Renderer/Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

namespace Engine
{
	namespace ModelData
	{
		struct Node
		{
			glm::mat4 transformation;
			Ptr<Bone> bone;
			int numOfchildren;
			std::vector<Ptr<Node>> children;

			Node(const aiNode* node, const Ptr<Animation> animation)
			{
				bone = animation->GetBoneByName(node->mName.data);
				transformation = AssimpUtil::ToGlm(node->mTransformation);
				numOfchildren = node->mNumChildren;
				for (int i = 0; i < node->mNumChildren; i++)
				{
					children.push_back(CreatePtr<Node>(node->mChildren[i], animation));
				}
			}
		};
	}

	class Model: public IModel
	{
	public:
		Model(std::string const& path);
		Model(std::string const& path, bool gamma);
		Model(std::string const& path, bool gamma, int entityId);
		virtual std::string& GetPath() override { return m_path; }
		virtual void UpdateAnimation(float deltaTime) override;
		virtual std::vector<glm::mat4> GetPoseTransforms() override { return m_transforms; }
		virtual void Draw() override;
		
	private:
		void Load(std::string const& path);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		Ptr<ModelData::Mesh> ProcessMesh(const aiMesh* mesh, const aiMaterial* material);
		std::vector<Ptr<Texture>> LoadMaterialTextures(const aiMaterial* material, aiTextureType type, TextureType textureType);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh);
		void CalculateBoneTransform(const Ptr<ModelData::Node> node, glm::mat4 parentTransform);

	private:
		std::string m_path;
		std::string m_directory;
		bool m_gammaCorrection;
		std::vector<Ptr<ModelData::Mesh>> m_meshes;
		std::vector<Ptr<Texture>> m_textures;
		Ptr<ModelData::Animation> m_animation;
		Ptr<ModelData::Node> m_rootNode;
		std::vector<glm::mat4> m_transforms;
		float m_animationTime = 0.0f;
		int m_entityId;
	};
}
