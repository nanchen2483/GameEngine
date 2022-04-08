#include "enginepch.h"
#include "AssimpModel.h"

#include "Engine/Renderer/Shader/Shader.h"
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
		: m_path(path), m_gammaCorrection(gamma), m_entityId(entityId), m_textureMap(textureMap)
	{
		m_transforms.reserve(100);
		for (int i = 0; i < 100; i++)
		{
			m_transforms.push_back(glm::mat4(1.0f));
		}

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

		m_directory = path.substr(0, path.find_last_of('\\'));
		if (scene->mNumAnimations)
		{
			m_includeAnimation = true;
			m_animation = CreatePtr<AssimpAnimation>(scene->mAnimations[0]);
			m_rootNode = CreatePtr<AssimpNode>(scene->mRootNode, m_animation);
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
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
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
		std::vector<uint32_t> indices;
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.position = AssimpUtil::ToGlm(mesh->mVertices[i]);
			vertex.normal = AssimpUtil::ToGlm(mesh->mNormals[i]);
			vertex.color = glm::vec4(1.0f);
			vertex.material = materialIndex;
			if (mesh->mTextureCoords[0])
			{
				vertex.texCoord = AssimpUtil::ToGlm(mesh->mTextureCoords[0][i]);
			}
			else
			{
				vertex.texCoord = glm::vec2(0.0f, 0.0f);
			}

			vertex.entityId = m_entityId;
			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		if (m_includeAnimation)
		{
			ExtractBoneWeightForVertices(vertices, mesh);
		}

		// return a mesh object created from the extracted mesh data
		return AssimpMesh(vertices, indices, textures);
	}

	Ptr<Texture> AssimpModel::LoadMaterialTexture(const aiMaterial* material, aiTextureType type, TextureType textureType)
	{
		if (material->GetTextureCount(type) > 0)
		{
			aiString filename;
			material->GetTexture(type, 0, &filename);
			std::string path = this->m_directory + '/' + filename.C_Str();
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

	void AssimpModel::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, const aiMesh* mesh)
	{
		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			aiBone* aiBone = mesh->mBones[boneIndex];
			std::string boneName = aiBone->mName.C_Str();
			Ptr<AssimpBone>& bone = m_animation->GetBoneByName(boneName);
			ENGINE_CORE_ASSERT(bone != nullptr, "Bone not found with boneName: {0}", boneName);
			bone->SetBoneOffset(AssimpUtil::ToGlm(aiBone->mOffsetMatrix));

			for (int weightIndex = 0; weightIndex < aiBone->mNumWeights; weightIndex++)
			{
				aiVertexWeight& vertexWeight = aiBone->mWeights[weightIndex];
				int vertexId = vertexWeight.mVertexId;
				float weight = vertexWeight.mWeight;
				ENGINE_CORE_ASSERT(vertexId <= vertices.size(), "Invalid vertexId: {0}", vertexId);

				Vertex& vertex = vertices[vertexId];
				for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
				{
					if (vertex.boneIds[i] == UNINITIALIZED_BONE_ID)
					{
						vertex.boneIds[i] = bone->GetBoneId();
						vertex.weights[i] = weight;
						break;
					}
				}
			}
		}
	}

	void AssimpModel::UpdateAnimation(float deltaTime)
	{
		if (m_includeAnimation)
		{
			m_animationTime = m_animation->GetAnimationTime(deltaTime);
			CalculateBoneTransform(m_rootNode, glm::mat4(1.0f));
		}
	}

	void AssimpModel::CalculateBoneTransform(const Ptr<AssimpNode>& node, glm::mat4 parentTransform)
	{
		glm::mat4 globalTransformation = parentTransform * node->transformation;
		Ptr<AssimpBone> bone = node->bone;
		if (bone != nullptr)
		{
			int boneId = bone->GetBoneId();
			glm::mat4 offset = bone->GetBoneOffset();
			globalTransformation = parentTransform * bone->GetLocalTransform(m_animationTime);
			m_transforms[boneId] = globalTransformation * offset;
		}

		for (int i = 0; i < node->numOfchildren; i++)
		{
			CalculateBoneTransform(node->children[i], globalTransformation);
		}
	}
	
	void AssimpModel::Draw()
	{
		for (uint32_t i = 0; i < m_meshes.size(); i++)
		{
			m_meshes[i].Draw();
		}
	}
}
