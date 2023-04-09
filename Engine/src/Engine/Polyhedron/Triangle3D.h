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
		Triangle3D(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC);

		const glm::dvec3& GetA() const { return m_vectorA; };
		const glm::dvec3& GetB() const { return m_vectorB; };
		const glm::dvec3& GetC() const { return m_vectorC; };

		const glm::dvec3& GetTopVertex() const { return m_vectorA; };
		const glm::dvec3& GetLeftBottomVertex() const { return m_vectorB; };
		const glm::dvec3& GetRighBottomtVertex() const { return m_vectorC; };

		const glm::dvec3& GetNormalVector() const { return m_normal; }
		const glm::dvec3& GetClosestPointToOrigin() const { return m_closestPointToOrigin; }
		const glm::dvec3& GetBarycentric(bool originEnclosed);
		
		void SetClosestPointToOrigin(bool originEnclosed);
	private:
		void InitNormalVector();
		glm::dvec3 CalcBarycentric(bool originEnclosed);
		glm::dvec3 CalcPoint(glm::dvec3 barycentric);
	protected:
		glm::dvec3 m_vectorA, m_vectorB, m_vectorC;
		glm::dvec3 m_normal;
		glm::dvec3 m_closestPointToOrigin;

		const double NUMERIC_EPSILON = 1e-12;
	};
}
