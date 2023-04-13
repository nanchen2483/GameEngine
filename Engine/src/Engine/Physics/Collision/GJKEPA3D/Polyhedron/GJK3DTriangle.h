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

		GJK3DTriangle* GetLeftTriangle() const { return m_leftTriangle; }
		GJK3DTriangle* GetRightTriangle() const { return m_rightTriangle; }
		GJK3DTriangle* GetBottomTriangle() const { return m_bottomTriangle; }

		const glm::dvec3& GetClosestPointToOrigin() const { return m_closestPointToOrigin; }
		const double GetClosestDistanceToOrigin() const { return m_closestDistanceToOrigin; }
		const double GetClosestDistanceToOriginSquare() const { return m_closestDistanceToOriginSquare; }
		const glm::dvec3 GetBarycentric(bool originEnclosed) const;

		void SetLeftTriangle(GJK3DTriangle* leftTriangle);
		void SetRightTriangle(GJK3DTriangle* rightTriangle);
		void SetBottomTriangle(GJK3DTriangle* bottomTriangle);
		void SetNeighbors(GJK3DTriangle* leftTriangle, GJK3DTriangle* rightTriangle, GJK3DTriangle* bottomTriangle);
		
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

		GJK3DTriangle* m_leftTriangle;
		GJK3DTriangle* m_rightTriangle;
		GJK3DTriangle* m_bottomTriangle;

		glm::dvec3 m_closestPointToOrigin;
		double m_closestDistanceToOrigin;
		double m_closestDistanceToOriginSquare;

		bool m_isDeleted;

		const double NUMERIC_EPSILON = 1e-12;
		const double NUMERIC_EPSILON_SQUARE = NUMERIC_EPSILON * NUMERIC_EPSILON;
	};
}
