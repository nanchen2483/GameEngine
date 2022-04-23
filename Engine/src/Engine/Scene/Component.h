#pragma once

#include "ScriptableEntity.h"
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Renderer/Model/Model.h"
#include "Engine/Renderer/Skybox/Skybox.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent& transform) = default;
		TagComponent(const std::string& tag)
			: tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent& transform) = default;
		TransformComponent(const glm::vec4& translation)
			: translation(translation) {}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), translation)
				* glm::toMat4(glm::quat(rotation))
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary;
		bool fixedAspectRatio = false;

		CameraComponent(bool isPrimary = true)
			: primary(isPrimary) {}
		CameraComponent(const CameraComponent& transform) = default;
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
		LightComponent(const LightComponent& renderer) = default;
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
		SpriteRendererComponent(const SpriteRendererComponent& renderer) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
		SpriteRendererComponent(Ptr<Texture2D> texture)
			: texture(texture) {}
	};

	struct ModelComponent
	{
		bool loading = false;
		Ptr<float> progression = CreatePtr<float>(0.0f);
		
		bool enableAnimation = false;
		Ptr<Model> model = nullptr;

		ModelComponent() = default;
		ModelComponent(const ModelComponent& modelComponent) = default;
		ModelComponent(const Ptr<Model>& model)
			: model(model) {}

		void OnUpdate(float deltaTime)
		{
			if (enableAnimation && model != nullptr)
			{
				model->OnUpdate(deltaTime);
			}
		}
	};

	struct SkyboxComponent
	{
		Ptr<Skybox> skybox = nullptr;
		std::vector<Ptr<Image>> images = std::vector<Ptr<Image>>(6);

		SkyboxComponent() = default;
		SkyboxComponent(const SkyboxComponent& skyboxComponent) = default;
		
		bool ReadyToLoad() { return std::find(images.begin(), images.end(), nullptr) == images.end(); }
		void SetFace(TextureOrientationType type, std::string filePath)
		{
			images[(uint32_t)type] = CreatePtr<Image>(filePath);
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
