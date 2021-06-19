#pragma once

#include "Engine/Scene/SceneCamera.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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
			glm::mat4 _rotation = glm::rotate(glm::mat4(1.0f), rotation.x, { 1, 0, 0 })
								* glm::rotate(glm::mat4(1.0f), rotation.y, { 0, 1, 0 })
								* glm::rotate(glm::mat4(1.0f), rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), translation)
				* _rotation
				* glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 color = glm::vec4(1.0f);

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent& renderer) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: color(color) {}
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
		
		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent* nsc);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};
}