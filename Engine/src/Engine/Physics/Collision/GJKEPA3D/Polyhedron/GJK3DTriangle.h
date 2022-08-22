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
	///       d
	/// </summary>
	class GJK3DTriangle : public Triangle3D
	{
	public:
		GJK3DTriangle(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c);

		double GetClosestDistanceToOrigin() { return glm::length(m_closestPointToOrigin); }
		double GetClosestDistanceToOriginSquare() { return glm::length2(m_closestPointToOrigin); }

		int32_t GetLeftTriangle() const { return m_leftTriangle; }
		int32_t GetRightTriangle() const { return m_rightTriangle; }
		int32_t GetBottomTriangle() const { return m_bottomTriangle; }
		int32_t GetPreviousTriangle() const { return m_previousTriangle; }
		int32_t GetNextTriangle() const { return m_nextTriangle; }

		void SetLeftTriangle(const uint32_t leftTriangle);
		void SetRightTriangle(const uint32_t rightTriangle);
		void SetBottomTriangle(const uint32_t bottomTriangle);
		void SetNeighbors(const uint32_t leftTriangle, const uint32_t rightTriangle, const uint32_t bottomTriangle);
		void SetPreviousTriangle(const uint32_t previousTriangle);
		void SetNextTriangle(const uint32_t nextTriangle);
		
		bool IsDeleted() const { return m_isDeleted; };
		void MarkAsDeleted();
	private:
		int32_t m_leftTriangle = -1;
		int32_t m_rightTriangle = -1;
		int32_t m_bottomTriangle = -1;
		
		int32_t m_previousTriangle = -1;
		int32_t m_nextTriangle = -1;

		bool m_isDeleted = false;
	};
}
