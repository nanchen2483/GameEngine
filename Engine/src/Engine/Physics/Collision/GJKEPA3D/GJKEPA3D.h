#pragma once
#include "Engine/Physics/Collision/Collision.h"
#include "Polyhedron/GJK3DDeltahedron.h"

namespace Engine
{
	class GJKEPA3D : public Collision
	{
	public:
		GJKEPA3D() = default;
		virtual const CollisionInfo Detect(const ShapeInfo& shapeA, const ShapeInfo& shapeB) override;
	private:
		const CollisionInfo Solve();
		glm::dvec3 CreateNewSupportPoint();
		glm::dvec3 GetPointFromShape(const ShapeInfo& shape, const glm::dvec3& direction);

		ShapeInfo m_shapeA, m_shapeB;
		Uniq<GJK3DDeltahedron> m_deltahedron = nullptr;

		const double SCALE = 1e-4;
		const glm::dvec3 V0 = SCALE * glm::dvec3(std::sqrt(8.0 / 9.0), 0.0, -1.0 / 3.0);
		const glm::dvec3 V1 = SCALE * glm::dvec3(-std::sqrt(2.0 / 9.0), std::sqrt(2.0 / 3.0), -1.0 / 3.0);
		const glm::dvec3 V2 = SCALE * glm::dvec3(-std::sqrt(2.0 / 9.0), -std::sqrt(2.0 / 3.0), -1.0 / 3.0);
		const glm::dvec3 V3 = SCALE * glm::dvec3(0.0f, 0.0f, 1.0f);
		const uint32_t MAX_ITERATIONS = 85;
	};
}
