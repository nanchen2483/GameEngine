#include "enginepch.h"
#include "AABB.h"
#include "Engine/Math/Math.h"

namespace Engine
{
	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
	{
		m_boundingValue.center = (max + min) * 0.5f;
		m_boundingValue.extents =
		{
			max.x - m_boundingValue.center.x,
			max.y - m_boundingValue.center.y,
			max.z - m_boundingValue.center.z
		};
		m_boundingValue.SupportMapping = [=](glm::vec3 direction)
		{
			glm::dvec3 result;
			result.x = Math::Sign(direction.x);
			result.y = Math::Sign(direction.y);
			result.z = Math::Sign(direction.z);

			return result;
		};
	}

	AABB::AABB(const glm::vec3& center, const float extentX, const float extentY, const float extentZ)
	{
		m_boundingValue.center = center;
		m_boundingValue.extents = glm::vec3(extentX, extentY, extentZ);
	}

	bool AABB::IsOnFrustum(const Frustum& frustum, const Transform& transform) const
	{
		// Convert center to world space
		const glm::vec3 worldCenter = (glm::mat4)transform * glm::vec4(m_boundingValue.center, 1.0f);

		const glm::quat orientation =		glm::quat(transform.rotation);
		const glm::vec3 rightDirection =	glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::vec3 upDirection =		glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::vec3 forwardDirection =	glm::rotate(orientation, glm::vec3(0.0f, 0.0f, -1.0f));

		// Scaled orientation
		const glm::vec3 right =		rightDirection * transform.scale.x * m_boundingValue.extents.x;
		const glm::vec3 up =		upDirection * transform.scale.y * m_boundingValue.extents.y;
		const glm::vec3 forward =	forwardDirection * transform.scale.z * m_boundingValue.extents.z;

		const float newExtentX =	std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
									std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
									std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

		const float newExtentY =	std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
									std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
									std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

		const float newExtentZ =	std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
									std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
									std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

		const AABB worldAABB(worldCenter, newExtentX, newExtentY, newExtentZ);

		return (worldAABB.IsOnForwardPlan(frustum.leftFace) &&
				worldAABB.IsOnForwardPlan(frustum.rightFace) &&
				worldAABB.IsOnForwardPlan(frustum.topFace) &&
				worldAABB.IsOnForwardPlan(frustum.bottomFace) &&
				worldAABB.IsOnForwardPlan(frustum.nearFace) &&
				worldAABB.IsOnForwardPlan(frustum.farFace));
	}

	bool AABB::IsOnForwardPlan(const Plan& plan) const
	{
		const float r = m_boundingValue.extents.x * std::abs(plan.normal.x) +
						m_boundingValue.extents.y * std::abs(plan.normal.y) +
						m_boundingValue.extents.z * std::abs(plan.normal.z);

		return -r <= plan.GetSignedDistanceToPlan(m_boundingValue.center);
	}
}
