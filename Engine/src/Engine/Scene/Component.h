#pragma once

#include "ScriptableEntity.h"
#include "Engine/Scene/SceneCamera.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Renderer/Model/Model.h"

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
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

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
		Ptr<Model> model = nullptr;

		ModelComponent() = default;
		ModelComponent(const ModelComponent& modelComponent) = default;
		ModelComponent(const Ptr<Model>& model)
			: model(model) {}
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
