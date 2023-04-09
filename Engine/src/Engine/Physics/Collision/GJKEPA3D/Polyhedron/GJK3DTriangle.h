#pragma once
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
	class GJK3DTriangle : public Triangle3D
	{
	public:
		GJK3DTriangle(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC);

		double GetClosestDistanceToOrigin() { return glm::length(m_closestPointToOrigin); }
		double GetClosestDistanceToOriginSquare() { return glm::length2(m_closestPointToOrigin); }

		GJK3DTriangle* GetLeftTriangle() const { return m_leftTriangle; }
		GJK3DTriangle* GetRightTriangle() const { return m_rightTriangle; }
		GJK3DTriangle* GetBottomTriangle() const { return m_bottomTriangle; }
		GJK3DTriangle* GetPreviousTriangle() const { return m_previousTriangle; }
		GJK3DTriangle* GetNextTriangle() const { return m_nextTriangle; }

		void SetLeftTriangle(GJK3DTriangle* leftTriangle);
		void SetRightTriangle(GJK3DTriangle* rightTriangle);
		void SetBottomTriangle(GJK3DTriangle* bottomTriangle);
		void SetNeighbors(GJK3DTriangle* leftTriangle, GJK3DTriangle* rightTriangle, GJK3DTriangle* bottomTriangle);
		void SetPreviousTriangle(GJK3DTriangle* previousTriangle);
		void SetNextTriangle(GJK3DTriangle* nextTriangle);
		
		bool IsDeleted() const { return m_isDeleted; };
		void MarkAsDeleted();
	private:
		GJK3DTriangle* m_leftTriangle = nullptr;
		GJK3DTriangle* m_rightTriangle = nullptr;
		GJK3DTriangle* m_bottomTriangle = nullptr;

		GJK3DTriangle* m_previousTriangle = nullptr;
		GJK3DTriangle* m_nextTriangle = nullptr;

		bool m_isDeleted = false;
	};
}
