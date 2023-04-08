#pragma once
#include "Engine/Physics/Collision/Collision.h"
#include "Polyhedron/GJK3DDeltahedron.h"

namespace Engine
{
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
		Vertex3D CreateNewSupportPoint();
		glm::dvec3 CalcPointA(const GJK3DTriangle* triangle, glm::dvec3 baryCentric);
		glm::dvec3 CalcPointB(const GJK3DTriangle* triangle, glm::dvec3 baryCentric);
		glm::dvec3 GetSupportPointOnA(glm::dvec3 direction);
		glm::dvec3 GetSupportPointOnB(glm::dvec3 direction);

		glm::dvec3 m_positionA = {};
		glm::dvec3 m_positionB = {};
		glm::dmat3 m_orientationA = {};
		glm::dmat3 m_orientationB = {};
		glm::dmat4 m_transformA = {};
		glm::dmat4 m_transformB = {};
		BoundingValue m_boundingValueA = {};
		BoundingValue m_boundingValueB = {};

		std::unordered_map<int32_t, glm::dvec3> m_pointOnAMap = {};
		std::unordered_map<int32_t, glm::dvec3> m_pointOnBMap = {};

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
