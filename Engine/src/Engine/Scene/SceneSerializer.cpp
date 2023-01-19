#include "enginepch.h"
#include "SceneSerializer.h"
#include "Component/AnimationComponent.h"
#include "Component/CameraComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/LightComponent.h"
#include "Component/MeshComponent.h"
#include "Component/SkyboxComponent.h"
#include "Component/SpriteRendererComponent.h"
#include "Component/TagComponent.h"
#include "Component/TerrainComponent.h"
#include "Component/TransformComponent.h"
#include "Engine/Library/ModelLibrary.h"
#include "Engine/Library/TextureLibrary.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace YAML {
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};
}

namespace Engine {

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& vector)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vector.x << vector.y << vector.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& vector)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << vector.x << vector.y << vector.z << vector.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ptr<Scene>& scene)
		: m_scene(scene)
	{
	}

	void static SerializeEntity(YAML::Emitter& out, Entity& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << 123;
		
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			
			std::string& tag = entity.GetComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			TransformComponent& transformComponent = entity.GetComponent<TransformComponent>();
			
			out << YAML::Key << "Translation" << YAML::Value << transformComponent.GetTranslation();
			out << YAML::Key << "Rotation" << YAML::Value << transformComponent.GetRotation();
			out << YAML::Key << "Scale" << YAML::Value << transformComponent.GetScale();

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
			SceneCamera& camera = cameraComponent.camera;

			out << YAML::Key << "Camera";
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.fixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<LightComponent>())
		{
			out << YAML::Key << "LightComponent";
			out << YAML::BeginMap;

			LightComponent& lightComponent = entity.GetComponent<LightComponent>();

			out << YAML::Key << "Constant" << YAML::Value << lightComponent.constant;
			out << YAML::Key << "Linear" << YAML::Value << lightComponent.linear;
			out << YAML::Key << "Quadratic" << YAML::Value << lightComponent.quadratic;

			out << YAML::Key << "Ambient" << YAML::Value << lightComponent.ambient;
			out << YAML::Key << "Diffuse" << YAML::Value << lightComponent.diffuse;
			out << YAML::Key << "Specular" << YAML::Value << lightComponent.specular;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			SpriteRendererComponent& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.color;
			if (spriteRendererComponent.texture != nullptr)
			{
				out << YAML::Key << "TextureFilePath" << YAML::Value << spriteRendererComponent.texture->GetFilePath();
			}

			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap;

			MeshComponent& meshComponent = entity.GetComponent<MeshComponent>();

			out << YAML::Key << "Path" << YAML::Value << meshComponent.filePath;
			out << YAML::Key << "IsPlayer" << YAML::Value << meshComponent.isPlayer;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<AnimationComponent>())
		{
			out << YAML::Key << "AnimationComponent";
			out << YAML::BeginMap;

			AnimationComponent& animationComponent = entity.GetComponent<AnimationComponent>();

			out << YAML::Key << "IsEnabled" << YAML::Value << animationComponent.isEnabled;
			out << YAML::Key << "SelectedAnimationIndex" << YAML::Value << animationComponent.selectedAnimationIndex;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CollisionComponent>())
		{
			out << YAML::Key << "CollisionComponent";
			out << YAML::BeginMap;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SkyboxComponent>())
		{
			out << YAML::Key << "SkyboxComponent";
			out << YAML::BeginMap;

			SkyboxComponent& skyboxComponent = entity.GetComponent<SkyboxComponent>();

			Ptr<Texture3D> texture = skyboxComponent.skybox->GetTexture();
			out << YAML::Key << "RightFilePath" << YAML::Value << texture->GetFilePath(TextureOrientationType::Right);
			out << YAML::Key << "LeftFilePath" << YAML::Value << texture->GetFilePath(TextureOrientationType::Left);
			out << YAML::Key << "TopFilePath" << YAML::Value << texture->GetFilePath(TextureOrientationType::Top);
			out << YAML::Key << "BottomFilePath" << YAML::Value << texture->GetFilePath(TextureOrientationType::Bottom);
			out << YAML::Key << "BackFilePath" << YAML::Value << texture->GetFilePath(TextureOrientationType::Back);
			out << YAML::Key << "FrontFilePath" << YAML::Value << texture->GetFilePath(TextureOrientationType::Front);

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TerrainComponent>())
		{
			out << YAML::Key << "TerrainComponent";
			out << YAML::BeginMap;

			TerrainComponent& terrainComponent = entity.GetComponent<TerrainComponent>();

			out << YAML::Key << "Type" << YAML::Value << std::to_string((uint32_t)terrainComponent.terrain->GetType());
			out << YAML::Key << "Path" << YAML::Value << terrainComponent.terrain->GetFilePath();
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}
	
	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_scene->m_registry.each([&](entt::entity entityId)
			{
				Entity entity = { entityId, m_scene.get() };
				if (!entity)
				{
					return;
				}

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		ENGINE_CORE_ASSERT("Not implemented", false);
	}
	
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();
		ENGINE_CORE_ASSERT("Deserializing scene '{0}'", sceneName);

		YAML::Node entities = data["Entities"];
		if (entities)
		{
			for (YAML::detail::iterator_value entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>();

				std::string name;
				YAML::Node tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}

				ENGINE_CORE_ASSERT("Deserialized entity with id = {0} and name = {1}", uuid, name);

				Entity deserializedEntity = m_scene->CreateEntity(name);

				YAML::Node transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					TransformComponent& deserializedTC = deserializedEntity.GetComponent<TransformComponent>();
					deserializedTC.transform.translation = transformComponent["Translation"].as<glm::vec3>();
					deserializedTC.transform.rotation = transformComponent["Rotation"].as<glm::vec3>();
					deserializedTC.transform.scale = transformComponent["Scale"].as<glm::vec3>();
				}

				YAML::Node cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					CameraComponent& deserializedCC = deserializedEntity.AddComponent<CameraComponent>();
					
					YAML::Node& cameraProps = cameraComponent["Camera"];
					deserializedCC.camera.SetProjectionType((CameraProjectionType)cameraProps["ProjectionType"].as<int>());

					deserializedCC.camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					deserializedCC.camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					deserializedCC.camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
					
					deserializedCC.camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					deserializedCC.camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					deserializedCC.camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					deserializedCC.primary = cameraComponent["Primary"].as<bool>();
					deserializedCC.fixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				YAML::Node lightComponent = entity["LightComponent"];
				if (lightComponent)
				{
					LightComponent& deserializedSRC = deserializedEntity.AddComponent<LightComponent>();

					deserializedSRC.constant = lightComponent["Constant"].as<float>();
					deserializedSRC.linear = lightComponent["Linear"].as<float>();
					deserializedSRC.quadratic = lightComponent["Quadratic"].as<float>();

					deserializedSRC.ambient = lightComponent["Ambient"].as<glm::vec3>();
					deserializedSRC.diffuse = lightComponent["Diffuse"].as<glm::vec3>();
					deserializedSRC.specular = lightComponent["Specular"].as<glm::vec3>();
				}

				YAML::Node spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					SpriteRendererComponent& deserializedSRC = deserializedEntity.AddComponent<SpriteRendererComponent>();

					deserializedSRC.color = spriteRendererComponent["Color"].as<glm::vec4>();
					YAML::Node& filePathNode = spriteRendererComponent["TextureFilePath"];
					if (filePathNode)
					{
						deserializedSRC.texture = TextureLibrary::GetInstance()->Load(filePathNode.as<std::string>());
					}
				}

				YAML::Node meshComponent = entity["MeshComponent"];
				if (meshComponent)
				{
					MeshComponent& deserializedMesh = deserializedEntity.AddComponent<MeshComponent>();

					deserializedMesh.filePath = meshComponent["Path"].as<std::string>();
					deserializedMesh.isPlayer = meshComponent["IsPlayer"].as<bool>();
					Ptr<Model> model = ModelLibrary::GetInstance()->Load(deserializedMesh.filePath);
					deserializedMesh.meshes = model->GetMeshes();

					YAML::Node animationComponent = entity["AnimationComponent"];
					if (animationComponent)
					{
						AnimationComponent& deserializeAnimation = deserializedEntity.AddComponent<AnimationComponent>();

						deserializeAnimation.isEnabled = animationComponent["IsEnabled"].as<bool>();
						deserializeAnimation.selectedAnimationIndex = animationComponent["SelectedAnimationIndex"].as<uint32_t>();
						deserializeAnimation.animations = model->GetAnimations();
					}

					YAML::Node collisionComponent = entity["CollisionComponent"];
					if (collisionComponent)
					{
						CollisionComponent& deserializedCollision = deserializedEntity.AddComponent<CollisionComponent>();

						deserializedCollision.boundingBox = model->GenerateBoundingBox();
					}
				}

				YAML::Node skyboxComponent = entity["SkyboxComponent"];
				if (skyboxComponent)
				{
					SkyboxComponent& deserializedSkybox = deserializedEntity.AddComponent<SkyboxComponent>();

					deserializedSkybox.images[(uint32_t)TextureOrientationType::Right] = CreatePtr<Image>(skyboxComponent["RightFilePath"].as<std::string>(), true);
					deserializedSkybox.images[(uint32_t)TextureOrientationType::Left] = CreatePtr<Image>(skyboxComponent["LeftFilePath"].as<std::string>(), true);
					deserializedSkybox.images[(uint32_t)TextureOrientationType::Top] = CreatePtr<Image>(skyboxComponent["TopFilePath"].as<std::string>(), true);
					deserializedSkybox.images[(uint32_t)TextureOrientationType::Bottom] = CreatePtr<Image>(skyboxComponent["BottomFilePath"].as<std::string>(), true);
					deserializedSkybox.images[(uint32_t)TextureOrientationType::Back] = CreatePtr<Image>(skyboxComponent["BackFilePath"].as<std::string>(), true);
					deserializedSkybox.images[(uint32_t)TextureOrientationType::Front] = CreatePtr<Image>(skyboxComponent["FrontFilePath"].as<std::string>(), true);

					deserializedSkybox.skybox = CreatePtr<Skybox>(deserializedSkybox.images);
				}

				YAML::Node terrainComponent = entity["TerrainComponent"];
				if (terrainComponent)
				{
					TerrainComponent& deserializedSkybox = deserializedEntity.AddComponent<TerrainComponent>();
					deserializedSkybox.texture = TextureLibrary::GetInstance()->Load(terrainComponent["Path"].as<std::string>(), TextureType::Height);
					deserializedSkybox.terrain = Terrain::Create((TerrainType)terrainComponent["Type"].as<int32_t>(), deserializedSkybox.texture, deserializedEntity);
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filkepath)
	{
		ENGINE_CORE_ASSERT("Not implemented", false);
		return false;
	}
}
