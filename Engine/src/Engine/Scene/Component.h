#pragma once

#include "Engine/Scene/SceneCamera.h"

#include <glm/glm.hpp>


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
		glm::mat4 transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent& transform) = default;
		TransformComponent(const glm::mat4& transform)
			: transform(transform) {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
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
}