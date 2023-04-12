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
		GJK3DTriangle(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC);

		GJK3DTriangle* GetLeftTriangle() const { return m_leftTriangle; }
		GJK3DTriangle* GetRightTriangle() const { return m_rightTriangle; }
		GJK3DTriangle* GetBottomTriangle() const { return m_bottomTriangle; }

		void SetLeftTriangle(GJK3DTriangle* const& leftTriangle);
		void SetRightTriangle(GJK3DTriangle* const& rightTriangle);
		void SetBottomTriangle(GJK3DTriangle* const& bottomTriangle);
		void SetNeighbors(GJK3DTriangle* const& leftTriangle, GJK3DTriangle* const& rightTriangle, GJK3DTriangle* const& bottomTriangle);
		
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
		GJK3DTriangle* m_leftTriangle;
		GJK3DTriangle* m_rightTriangle;
		GJK3DTriangle* m_bottomTriangle;

		bool m_isDeleted;
	};
}
