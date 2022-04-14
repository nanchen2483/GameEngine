#include "enginepch.h"
#include "AssimpModel.h"

#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Util/AssimpUtil.h"

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <stb_image.h>

namespace Engine
{
	AssimpModel::AssimpModel(std::string const& path)
		: AssimpModel(path, false)
	{
	}

	AssimpModel::AssimpModel(std::string const& path, bool gamma)
		: AssimpModel(path, gamma, -1, &TextureMap())
	{
	}

	AssimpModel::AssimpModel(std::string const& path, bool gamma, int entityId, TextureMap* textureMap)
		: m_filePath(path), m_gammaCorrection(gamma), m_entityId(entityId), m_textureMap(textureMap)
	{
		Load(path);
	}

	void AssimpModel::Load(std::string const& path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		// check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
			return;
		}

		if (scene->HasAnimations())
		{
			m_animation = CreatePtr<AssimpAnimation>(scene);
		}

		ProcessNode(scene->mRootNode, scene);
	}

	void AssimpModel::ProcessNode(const aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			m_meshes.push_back(ProcessMesh(mesh, material));
		}

		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	AssimpMesh AssimpModel::ProcessMesh(const aiMesh* mesh, const aiMaterial* material)
	{
		glm::vec3 materialIndex = glm::vec3(-1, -1, 64.0f);
		std::vector<Ptr<Texture>> textures;
		Ptr<Texture> diffuseTexture = LoadMaterialTexture(material, aiTextureType::aiTextureType_DIFFUSE, TextureType::Diffuse);
		if (diffuseTexture)
		{
			materialIndex.x += 1;
			textures.push_back(diffuseTexture);
		}

		Ptr<Texture> specularTexture = LoadMaterialTexture(material, aiTextureType::aiTextureType_SPECULAR, TextureType::Specular);
		if (specularTexture)
		{
			materialIndex.y = materialIndex.x + 1;
			textures.push_back(specularTexture);
		}

		Ptr<Texture> normalTexture = LoadMaterialTexture(material, aiTextureType::aiTextureType_HEIGHT, TextureType::Normal);
		if (normalTexture)
		{
			textures.push_back(normalTexture);
		}
		
		Ptr<Texture> heightTexture = LoadMaterialTexture(material, aiTextureType::aiTextureType_AMBIENT, TextureType::Height);
		if (heightTexture)
		{
			textures.push_back(heightTexture);
		}

		std::vector<Vertex> vertices;
		vertices.reserve(mesh->mNumVertices);
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = AssimpUtil::ToGlm(mesh->mVertices[i]);
			vertex.normal = AssimpUtil::ToGlm(mesh->mNormals[i]);
			vertex.color = glm::vec4(1.0f);
			vertex.material = materialIndex;
			vertex.texCoord = mesh->HasTextureCoords(0) ? AssimpUtil::ToGlm(mesh->mTextureCoords[0][i]) : glm::vec2(0.0f);
			vertex.entityId = m_entityId;
			vertices.push_back(vertex);
		}

		std::vector<uint32_t> indices;
		indices.reserve((mesh->mNumFaces * 3));
		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			const aiFace face = mesh->mFaces[i];
			ENGINE_CORE_ASSERT(face.mNumIndices == 3, "Number of indices must be 3!");
			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		ExtractBoneWeight(vertices, mesh);

		// return a mesh object created from the extracted mesh data
		return AssimpMesh(vertices, indices, textures);
	}

	Ptr<Texture> AssimpModel::LoadMaterialTexture(const aiMaterial* material, aiTextureType type, TextureType textureType)
	{
		if (material->GetTextureCount(type) > 0)
		{
			aiString filename;
			material->GetTexture(type, 0, &filename);
			std::string path = (this->m_directory /+ filename.C_Str()).string();
			Ptr<Texture> materialTexture = (*m_textureMap)[path];
			if (materialTexture == nullptr)
			{
				materialTexture = Texture2D::Create(path, textureType, false);
				(*m_textureMap)[path] = materialTexture;  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}

			return materialTexture;
		}

		return nullptr;
	}

	void AssimpModel::ExtractBoneWeight(std::vector<Vertex>& vertices, const aiMesh* mesh)
	{
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
		{
			const aiBone* aiBone = mesh->mBones[boneIndex];
			const uint32_t boneId = m_animation->GetBoneId(aiBone->mName.data);
			for (uint32_t weightIndex = 0; weightIndex < aiBone->mNumWeights; weightIndex++)
			{
				const aiVertexWeight& vertexWeight = aiBone->mWeights[weightIndex];
				const uint32_t vertexId = vertexWeight.mVertexId;
				ENGINE_CORE_ASSERT(vertexId <= vertices.size(), "Invalid vertexId: {0}", vertexId);
				vertices[vertexId].SetBone(boneId, vertexWeight.mWeight);
			}
		}
	}

	void AssimpModel::UpdateAnimation(float deltaTime)
	{
		if (m_animation != nullptr)
		{
			m_animation->UpdateBoneTransform(deltaTime);
		}
	}

	std::vector<glm::mat4> AssimpModel::GetPoseTransforms()
	{
		if (m_animation != nullptr)
		{
			return m_animation->GetBoneTransforms();
		}

		return std::vector<glm::mat4>();
	}
	
	void AssimpModel::Draw()
	{
		for (uint32_t i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].Draw();
		}
	}
}
