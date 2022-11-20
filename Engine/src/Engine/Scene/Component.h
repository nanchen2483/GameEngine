#pragma once
#include "Engine/Math/Transform.h"
#include "Engine/Renderer/Model/Model.h"
#include "Engine/Renderer/Skybox/Skybox.h"
#include "Engine/Renderer/Terrain/Terrain.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Scene/SceneCamera.h"
#include "ScriptableEntity.h"

namespace Engine
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent& component) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}
	};

	struct TransformComponent
	{
		Transform transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& component) = default;
		TransformComponent(const Transform& transform)
			: transform(transform) {}

		glm::vec3 GetTranslation() const { return transform.translation; }
		glm::vec3 GetRotation() const { return transform.rotation; }
		glm::vec3 GetScale() const { return transform.scale; }
		glm::mat4 GetViewMatrix() const { return glm::inverse((glm::mat4)transform); }
		
		operator glm::mat4() const { return transform; }
		operator Transform&() { return transform; }
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary;
		bool fixedAspectRatio = false;

		CameraComponent(bool isPrimary = true)
			: primary(isPrimary) {}
		CameraComponent(const CameraComponent& component) = default;
	};

	struct LightComponent
	{
		float constant = 1.0f;
		float linear = 0.1f;
		float quadratic = 0.03f;

		glm::vec3 position = glm::vec3(1.0f);
		glm::vec3 ambient = glm::vec3(1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f);
		glm::vec3 specular = glm::vec3(1.0f);

		LightComponent() = default;
		LightComponent(const LightComponent& component) = default;
		LightComponent(const float constant, const float linear, const float quadratic,
					   const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
			: constant(constant), linear(linear), quadratic(quadratic),
				ambient(ambient), diffuse(diffuse), specular(specular) {}

		const std::vector<const void *> GetData() const
		{
			return { &position, &constant, &linear, &quadratic, &ambient, &diffuse, &specular };
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color = glm::vec4(1.0f);
		Ptr<Texture2D> texture = nullptr;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& component) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
		SpriteRendererComponent(Ptr<Texture2D> texture)
			: texture(texture) {}
	};

	struct ModelComponent
	{
		bool loading = false;
		Ptr<float> progression = CreatePtr<float>(0.0f);
		bool isOnViewFrustum = false;
		
		bool enableAnimation = false;
		Ptr<Model> model = nullptr;
		
		ModelComponent() = default;
		ModelComponent(const ModelComponent& component) = default;
		ModelComponent(const Ptr<Model>& model)
			: model(model) {}

		BoundingValue GetBoundingValue()
		{
			if (model != nullptr)
			{
				return model->GetBoundingValue();
			}

			return {};
		}

		void OnUpdate(float deltaTime, const Frustum& frustum, const Transform& transform)
		{
			bool isOnViewFrustum = IsOnViewFrustum(frustum, transform);
			if (isOnViewFrustum && enableAnimation && model != nullptr)
			{
				model->OnUpdate(deltaTime);
			}
		}

		operator BoundingValue() { return GetBoundingValue(); }
		operator bool() { return model != nullptr; }
	private:
		bool IsOnViewFrustum(const Frustum& frustum, const Transform& transform)
		{
			isOnViewFrustum = false;
			if (model != nullptr)
			{
				isOnViewFrustum = model->IsOnFrustum(frustum, transform);
			}

			return isOnViewFrustum;
		}
	};

	struct SkyboxComponent
	{
		Ptr<Skybox> skybox = nullptr;
		std::vector<Ptr<Image>> images = std::vector<Ptr<Image>>(6);

		SkyboxComponent() = default;
		SkyboxComponent(const SkyboxComponent& component) = default;
		
		bool ReadyToLoad() { return std::find(images.begin(), images.end(), nullptr) == images.end(); }
		void SetFace(TextureOrientationType type, std::string filePath)
		{
			images[(uint32_t)type] = CreatePtr<Image>(filePath, false);
			textures[(uint32_t)type] = Texture2D::Create(images[(uint32_t)type]);
			if (ReadyToLoad())
			{
				if (skybox == nullptr)
				{
					skybox = CreatePtr<Skybox>(Texture3D::Create(images));
				}
				else
				{
					skybox->SetTexture(Texture3D::Create(images));
				}
			}
		}
		
		// Editor-only
		uint32_t GetTextureId(TextureOrientationType type, uint32_t defaultValue)
		{
			Ptr<Texture2D> texture = textures[(uint32_t)type];
			if (texture != nullptr)
			{
				return texture->GetRendererId();
			}
			else
			{
				return defaultValue;
			}
		}
	private:
		// Editor-only
		std::vector<Ptr<Texture2D>> textures = std::vector<Ptr<Texture2D>>(6);
	};

	struct TerrainComponent
	{
		Ptr<Terrain> terrain = nullptr;
		Ptr<Texture2D> texture = nullptr;

		TerrainComponent() = default;
		TerrainComponent(const TerrainComponent& component) = default;
		TerrainComponent(const Ptr<Terrain>& terrain)
			: terrain(terrain) {}

		void SetHeight(TransformComponent& translate)
		{
			if (terrain != nullptr)
			{
				translate.transform.translation.y = terrain->GetHeight(translate.GetTranslation().x, translate.GetTranslation().z);
			}
		}

		void OnUpdate(glm::vec3 cameraPosition)
		{
			if (terrain != nullptr)
			{
				terrain->OnUpdate(cameraPosition);
			}
		}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* instance = nullptr;
		
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent* nsc);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};
}
