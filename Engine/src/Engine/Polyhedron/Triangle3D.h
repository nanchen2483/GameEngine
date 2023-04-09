#pragma once
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
		Triangle3D(const glm::dvec3& pointA, const glm::dvec3& pointB, const glm::dvec3& pointC);

		glm::dvec3 GetA() const { return m_pointA; };
		glm::dvec3 GetB() const { return m_pointB; };
		glm::dvec3 GetC() const { return m_pointC; };

		glm::dvec3 GetTopVertex() const { return m_pointA; };
		glm::dvec3 GetLeftBottomVertex() const { return m_pointB; };
		glm::dvec3 GetRighBottomtVertex() const { return m_pointC; };

		glm::dvec3 GetNormalVector() const { return m_normal; }
		glm::dvec3 GetClosestPointToOrigin() const { return m_closestPointToOrigin; }
		glm::dvec3 GetBarycentric(bool originEnclosed);
		
		void SetClosestPointToOrigin(bool originEnclosed);
	private:
		void InitNormalVector();
		glm::dvec3 CalcBarycentric(bool originEnclosed);
		glm::dvec3 CalcPoint(glm::dvec3 barycentric);
	protected:
		glm::dvec3 m_pointA, m_pointB, m_pointC;
		glm::dvec3 m_normal;
		glm::dvec3 m_closestPointToOrigin;

		const double NUMERIC_EPSILON = 1e-12;
	};
}
