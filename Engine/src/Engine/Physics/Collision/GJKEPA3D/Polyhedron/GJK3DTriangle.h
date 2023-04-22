#pragma once
#include "Engine/Core/System/Object/IComparable.h"
#include "Engine/Polyhedron/Triangle3D.h"

namespace Engine
{
	/// <summary>
	/// d --- a --- d
	///  \ R / \ L /	F: front triangle (current)
	///   \ / F \ /		L: left triangle
	///    c --- b		R: right triangle
	///     \ B /		B: bottom triangle
	///      \ /
	///       d			Points: a, b, c, d
	/// </summary>
	class GJK3DTriangle : public Triangle3D, IComparable<GJK3DTriangle>
	{
	public:
		GJK3DTriangle(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC, bool originEnclosed);

		const Ptr<GJK3DTriangle> GetLeftTriangle() const { return m_leftTriangle.lock(); }
		const Ptr<GJK3DTriangle> GetRightTriangle() const { return m_rightTriangle.lock(); }
		const Ptr<GJK3DTriangle> GetBottomTriangle() const { return m_bottomTriangle.lock(); }

		const glm::dvec3& GetClosestPointToOrigin() const { return m_closestPointToOrigin; }
		const double GetClosestDistanceToOrigin() const { return m_closestDistanceToOrigin; }
		const double GetClosestDistanceToOriginSquare() const { return m_closestDistanceToOriginSquare; }
		const glm::dvec3 GetBarycentric(bool originEnclosed) const;

		void SetLeftTriangle(const Ptr<GJK3DTriangle>& leftTriangle);
		void SetRightTriangle(const Ptr<GJK3DTriangle>& rightTriangle);
		void SetBottomTriangle(const Ptr<GJK3DTriangle>& bottomTriangle);
		void SetNeighbors(const Ptr<GJK3DTriangle>& leftTriangle, const Ptr<GJK3DTriangle>& rightTriangle, const Ptr<GJK3DTriangle>& bottomTriangle);
		
		bool IsExpandable(const glm::dvec3& point) const;
		bool IsDeleted() const { return m_isDeleted; };
		void MarkAsDeleted();

		// Inherited via IComparable
		virtual bool operator==(const GJK3DTriangle& other) const override;
		virtual bool operator!=(const GJK3DTriangle& other) const override;
		virtual bool operator<(const GJK3DTriangle& other) const override;
		virtual bool operator<=(const GJK3DTriangle& other) const override;
		virtual bool operator>(const GJK3DTriangle& other) const override;
		virtual bool operator>=(const GJK3DTriangle& other) const override;
	private:
		void InitClosestPointToOrigin(bool originEnclosed);

		Weak<GJK3DTriangle> m_leftTriangle;
		Weak<GJK3DTriangle> m_rightTriangle;
		Weak<GJK3DTriangle> m_bottomTriangle;

		glm::dvec3 m_closestPointToOrigin;
		double m_closestDistanceToOrigin;
		double m_closestDistanceToOriginSquare;

		bool m_isDeleted;

		const double NUMERIC_EPSILON = 1e-12;
		const double NUMERIC_EPSILON_SQUARE = NUMERIC_EPSILON * NUMERIC_EPSILON;
	};
}
