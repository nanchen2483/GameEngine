#include "enginepch.h"
#include "Triangle3D.h"

namespace Engine
{
	Triangle3D::Triangle3D(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC)
		: m_vectorA(vectorA), m_vectorB(vectorB), m_vectorC(vectorC)
	{
		InitNormalVector();
	}

	void Triangle3D::InitNormalVector()
	{
		glm::dvec3 u = m_vectorA - m_vectorB;
		glm::dvec3 v = m_vectorA - m_vectorC;
		m_normal = glm::cross(u, v);
	}

	const glm::dvec3& Triangle3D::GetBarycentric(bool originEnclosed)
	{
		return CalcBarycentric(originEnclosed);
	}

	void Triangle3D::SetClosestPointToOrigin(bool originEnclosed)
	{
		glm::dvec3 barycentric = CalcBarycentric(originEnclosed);
		if (glm::length2(m_normal) > NUMERIC_EPSILON * NUMERIC_EPSILON)
		{
			if (originEnclosed)
			{
				double delta = glm::dot(m_normal, (glm::dvec3)m_vectorA);
				m_closestPointToOrigin = m_normal * (delta / glm::length2(m_normal));
			}
			else
			{
				m_closestPointToOrigin = CalcPoint(barycentric);
			}
		}
	}

	glm::dvec3 Triangle3D::CalcBarycentric(bool originEnclosed)
	{
		glm::dvec3 u = m_vectorA - m_vectorB;
		glm::dvec3 v = m_vectorA - m_vectorC;

		double t = glm::length2(m_normal);
		glm::dvec3 direction = glm::cross(u, (glm::dvec3)m_vectorA);
		double gamma = glm::dot(direction, m_normal) / t;
		direction = glm::cross((glm::dvec3)m_vectorA, v);
		double beta = glm::dot(direction, m_normal) / t;
		double alpha = 1.0 - gamma - beta;

		if (!originEnclosed)
		{
			if (alpha >= 0.0 && beta < 0.0)
			{
				t = glm::dot((glm::dvec3)m_vectorA, u);
				if ((gamma < 0.0) && (t > 0.0))
				{
					beta = std::min(1.0, t / glm::length2(u));
					alpha = 1.0 - beta;
					gamma = 0.0;
				}
				else
				{
					gamma = std::min(1.0, std::max(0.0, glm::dot((glm::dvec3)m_vectorA, v) / glm::length2(v)));
					alpha = 1.0 - gamma;
					beta = 0.0;
				}
			}
			else if ((beta >= 0.0) && (gamma < 0.0))
			{
				glm::dvec3 w = m_vectorB - m_vectorC;
				t = glm::dot((glm::dvec3)m_vectorB, w);
				if ((alpha < 0.0) && (t > 0.0))
				{
					gamma = std::min(1.0, t / glm::length2(w));
					beta = 1.0 - gamma;
					alpha = 0.0;
				}
				else
				{
					alpha = std::min(1.0, std::max(0.0, -glm::dot((glm::dvec3)m_vectorB, u) / glm::length2(u)));
					beta = 1.0 - alpha;
					gamma = 0.0;
				}
			}
			else if ((gamma >= 0.0) && (alpha < 0.0))
			{
				glm::dvec3 w = m_vectorB - m_vectorC;
				t = -glm::dot((glm::dvec3)m_vectorC, v);
				if ((beta < 0.0) && (t > 0.0))
				{
					alpha = std::min(1.0, t / glm::length2(v));
					gamma = 1.0 - alpha;
					beta = 0.0;
				}
				else
				{
					beta = std::min(1.0, std::max(0.0, -glm::dot((glm::dvec3)m_vectorC, w) / glm::length2(w)));
					gamma = 1.0 - beta;
					alpha = 0.0;
				}
			}
		}

		return glm::dvec3(alpha, beta, gamma);
	}

	glm::dvec3 Triangle3D::CalcPoint(glm::dvec3 barycentric)
	{
		glm::dmat3 matrix(m_vectorA, m_vectorB, m_vectorC);

		return matrix * barycentric;
	}
}
