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
	protected:
		glm::dvec3 m_vectorA, m_vectorB, m_vectorC;

		glm::dvec3 m_normal;
		double m_normalLengthSquare;
	};
}
