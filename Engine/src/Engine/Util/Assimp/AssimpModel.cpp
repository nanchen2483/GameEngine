#include "enginepch.h"
#include "AssimpModel.h"
#include "AssimpMesh.h"
#include "Engine/Library/TextureLibrary.h"
#include "Engine/Scene/System/AnimationSystem.h"

#include <stb_image.h>

namespace Engine
{
	AssimpModel::AssimpModel(std::string const& path)
		: AssimpModel(path, false)
	{
	}

	AssimpModel::AssimpModel(std::string const& path, bool gamma)
		: AssimpModel(path, gamma, nullptr)
	{
	}

	AssimpModel::AssimpModel(std::string const& path, bool gamma, Ptr<float> progression)
		: m_filePath(path), m_gammaCorrection(gamma), m_progression(progression)
	{
		m_uid = Uid::NewUid(path);

		Load(path);
	}

	Ptr<BoundingBox> AssimpModel::GenerateBoundingBox()
	{
		return BoundingBox::Create(BoundingBoxType::AABB, m_minAABB, m_maxAABB);
	}

	void AssimpModel::Load(std::string const& path)
	{
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_LimitBoneWeights |
			aiProcess_OptimizeMeshes |
			aiProcess_FlipUVs);
		// Check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			ENGINE_CORE_ERROR("Could not import model: {0} with error message: {1}", path, importer.GetErrorString());
			return;
		}

		m_totalProgression = (float)(scene->mNumMaterials + scene->mNumMeshes + scene->mNumAnimations);

		LoadMeshes(scene);
		LoadAnimations(scene);
	}

	void AssimpModel::LoadMeshes(const aiScene* scene)
	{
		std::vector<Material> materials;
		materials.reserve(scene->mNumMaterials);
		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
		{
			const aiMaterial* material = scene->mMaterials[materialIndex];
			Material currentMaterial;
			currentMaterial.diffusePath		= GetTexturePath(material, aiTextureType::aiTextureType_DIFFUSE, TextureType::Diffuse);
			currentMaterial.specularPath	= GetTexturePath(material, aiTextureType::aiTextureType_SPECULAR, TextureType::Specular);
			currentMaterial.normalPath		= GetTexturePath(material, aiTextureType::aiTextureType_HEIGHT, TextureType::Normal);
			currentMaterial.heightPath		= GetTexturePath(material, aiTextureType::aiTextureType_AMBIENT, TextureType::Height);
			
			materials.push_back(currentMaterial);
			IncreaseProgression();
		}

		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			Material material = materials[mesh->mMaterialIndex];
			std::vector<Vertex> vertices;
			vertices.reserve(mesh->mNumVertices);
			for (uint32_t i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				vertex.position = AssimpUtil::ToGlm(mesh->mVertices[i]);
				vertex.normal = AssimpUtil::ToGlm(mesh->mNormals[i]);
				vertex.color = glm::vec4(1.0f);
				vertex.material = material.GetTextureSlotIndices();
				vertex.texCoord = glm::vec2(0.0f);
				if (mesh->HasTextureCoords(0))
				{
					vertex.texCoord = AssimpUtil::ToGlm(mesh->mTextureCoords[0][i]);
				}

				vertices.push_back(vertex);

				UpdateBoundingValues(vertex.position);
			}

			uint32_t numOfIndices = mesh->mNumFaces * 3;
			std::vector<uint32_t> indices;
			indices.reserve(numOfIndices);
			for (uint32_t i = 0; i < mesh->mNumFaces; i++)
			{
				const aiFace face = mesh->mFaces[i];
				ENGINE_CORE_ASSERT(face.mNumIndices == 3, "Number of indices must be 3!");
				for (uint32_t j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			LoadBones(vertices, mesh);

			m_meshes.push_back(CreatePtr<AssimpMesh>(vertices, indices, material));
			IncreaseProgression();
		}
	}

	void AssimpModel::LoadAnimations(const aiScene* scene)
	{
		if (scene->HasAnimations())
		{
			for (uint32_t i = 0; i < scene->mNumAnimations; i++)
			{
				Ptr<Node> rootNode = AssimpHelper::ConvertToAssimpNode(scene->mAnimations[i], scene->mRootNode, m_boneOffsets);
				Ptr<Animation> animation = CreatePtr<AssimpAnimation>(scene->mAnimations[i], rootNode);
				m_animations.push_back(animation);

				IncreaseProgression();
			}

			// Initial bone transform-matrix
			AnimationSystem::UpdateAnimation(m_animations.front());
		}
	}

	void AssimpModel::LoadBones(std::vector<Vertex>& vertices, const aiMesh* mesh)
	{
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
		{
			const aiBone* aiBone = mesh->mBones[boneIndex];
			const auto& bone = m_boneOffsets.SafeAdd(aiBone->mName.data, AssimpUtil::ToGlm(aiBone->mOffsetMatrix));
			for (uint32_t weightIndex = 0; weightIndex < aiBone->mNumWeights; weightIndex++)
			{
				const aiVertexWeight& vertexWeight = aiBone->mWeights[weightIndex];
				const uint32_t vertexId = vertexWeight.mVertexId;
				ENGINE_CORE_ASSERT(vertexId <= vertices.size(), "Invalid vertexId: {0}", vertexId);
				if (!vertices[vertexId].SetBone(bone.id, vertexWeight.mWeight))
				{
					// No empty space
					break;
				}
			}
		}
	}

	std::string AssimpModel::GetTexturePath(const aiMaterial* material, const aiTextureType type, const TextureType textureType)
	{
		if (material->GetTextureCount(type) > 0)
		{
			aiString filename;
			material->GetTexture(type, 0, &filename);
			std::string path = (this->m_directory /+ filename.C_Str()).string();

			return path;
		}

		return {};
	}

	void AssimpModel::UpdateBoundingValues(glm::vec3 position)
	{
		m_minAABB.x = std::min(m_minAABB.x, position.x);
		m_minAABB.y = std::min(m_minAABB.y, position.y);
		m_minAABB.z = std::min(m_minAABB.z, position.z);

		m_maxAABB.x = std::max(m_maxAABB.x, position.x);
		m_maxAABB.y = std::max(m_maxAABB.y, position.y);
		m_maxAABB.z = std::max(m_maxAABB.z, position.z);
	}

	void AssimpModel::IncreaseProgression()
	{
		if (m_progression != nullptr)
		{
			m_currentProgression++;
			(*m_progression) = (m_currentProgression / m_totalProgression);
		}
	}
}
