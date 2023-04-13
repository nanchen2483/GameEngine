#include "enginepch.h"
#include "Triangle3D.h"

namespace Engine
{
	Triangle3D::Triangle3D(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC)
		: m_vectorA(vectorA)
		, m_vectorB(vectorB)
		, m_vectorC(vectorC)
	{
		glm::dvec3 u = m_vectorA - m_vectorB;
		glm::dvec3 v = m_vectorA - m_vectorC;
		m_normal = glm::cross(u, v);
		m_normalLengthSquare = glm::length2(m_normal);
	}
}
