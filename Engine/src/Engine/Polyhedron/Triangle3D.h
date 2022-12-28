#pragma once
#include "Vertex3D.h"

#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	/// <summary>
	///		(a)
	///    /   \
	///   /		\
	/// (c)-----(b)
	class Triangle3D
	{
	public:
		Triangle3D(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c);

		Vertex3D GetA() const { return m_a; };
		Vertex3D GetB() const { return m_b; };
		Vertex3D GetC() const { return m_c; };

		Vertex3D GetTopVertex() const { return m_a; };
		Vertex3D GetLeftBottomVertex() const { return m_b; };
		Vertex3D GetRighBottomtVertex() const { return m_c; };

		glm::dvec3 GetNormalVector() const { return m_normal; }
		glm::dvec3 GetClosestPointToOrigin() const { return m_closestPointToOrigin; }
		glm::dvec3 GetBarycentric(bool originEnclosed);
		
		void SetClosestPointToOrigin(bool originEnclosed);
	private:
		void InitNormalVector();
		glm::dvec3 CalcBarycentric(bool originEnclosed);
		glm::dvec3 CalcPoint(glm::dvec3 barycentric);
	protected:
		Vertex3D m_a, m_b, m_c;
		glm::dvec3 m_normal;
		glm::dvec3 m_closestPointToOrigin;

		const double NUMERIC_EPSILON = 1e-12;
	};
}
