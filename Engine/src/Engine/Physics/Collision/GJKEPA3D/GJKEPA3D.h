#pragma once
#include "Engine/Physics/Collision/Collision.h"
#include "Engine/Util/Vector3DHash.h"
#include "Polyhedron/GJK3DDeltahedron.h"

namespace Engine
{
	struct ShapeInfo
	{
		glm::dvec3 position = {};
		glm::dmat3 orientation = {};
		glm::dmat4 transform = {};
		BoundingValue boundingValue = {};
		std::unordered_map<glm::dvec3, glm::dvec3, Vector3DHash> pointMap = {};
	};

	class GJKEPA3D : public Collision
	{
	public:
		GJKEPA3D() = default;

		virtual bool Detect(Transform transformA, Transform transformB, BoundingValue boundingValueA, BoundingValue boundingValueB) override;
		virtual bool IsCollided() const override { return m_distanceBetweenAToB < 0.0f; }
		virtual float GetDistanceBetweenAAndB() const override { return m_distanceBetweenAToB; }
		virtual glm::vec3 GetDirectionFromAToB() const override { return m_directionFromAToB; }
	private:
		bool Solve();
		glm::dvec3 CreateNewSupportPoint();
		glm::dvec3 GetDistanceBetweenShapes();
		glm::dvec3 GetPointFromShape(ShapeInfo& shape, const GJK3DTriangle* triangle, glm::dvec3 baryCentric);
		glm::dvec3 GetPointFromShape(const ShapeInfo& shape, glm::dvec3 direction);

		ShapeInfo m_shapeA, m_shapeB;

		float m_distanceBetweenAToB = 0;
		glm::vec3 m_directionFromAToB = {};

		Uniq<GJK3DDeltahedron> m_deltahedron = nullptr;

		const double SCALE = 1e-4;
		const glm::dvec3 V0 = SCALE * glm::dvec3(std::sqrt(8.0 / 9.0), 0.0, -1.0 / 3.0);
		const glm::dvec3 V1 = SCALE * glm::dvec3(-std::sqrt(2.0 / 9.0), std::sqrt(2.0 / 3.0), -1.0 / 3.0);
		const glm::dvec3 V2 = SCALE * glm::dvec3(-std::sqrt(2.0 / 9.0), -std::sqrt(2.0 / 3.0), -1.0 / 3.0);
		const glm::dvec3 V3 = SCALE * glm::dvec3(0.0f, 0.0f, 1.0f);
		const uint32_t MAX_ITERATION = 85;
	};
}
