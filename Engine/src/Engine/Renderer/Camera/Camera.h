#pragma once
#include "Engine/Math/Transform.h"
#include "Frustum.h"

#include <glm/glm.hpp>

namespace Engine
{
	class Camera
	{
	public:
		enum class CameraType
		{
			None = 0,
			FocusPoint,
			FreeLook,
		};
		enum class ProjectionType
		{
			Perspective = 1,
			Orthographic,
		};
	public:
		virtual ~Camera() = default;

		virtual const glm::mat4& GetProjection() const = 0;
		virtual const float GetFOV() const = 0;
		virtual const float GetAspectRatio() const = 0;
		virtual Frustum GetFrustum(const Transform& transform) const = 0;
		virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;
	};
}
