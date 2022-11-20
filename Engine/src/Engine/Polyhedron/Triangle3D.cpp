#include "enginepch.h"
#include "Triangle3D.h"

namespace Engine
{
	Triangle3D::Triangle3D(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c)
		: m_a(a), m_b(b), m_c(c)
	{
		InitNormalVector();
	}

	void Triangle3D::InitNormalVector()
	{
		glm::dvec3 u = m_a - m_b;
		glm::dvec3 v = m_a - m_c;
		m_normal = glm::cross(u, v);
	}

	glm::dvec3 Triangle3D::GetBarycentric(bool originEnclosed)
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
				double delta = glm::dot(m_normal, (glm::dvec3)m_a);
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
		glm::dvec3 u = m_a - m_b;
		glm::dvec3 v = m_a - m_c;

		double t = glm::length2(m_normal);
		glm::dvec3 direction = glm::cross(u, (glm::dvec3)m_a);
		double gamma = glm::dot(direction, m_normal) / t;
		direction = glm::cross((glm::dvec3)m_a, v);
		double beta = glm::dot(direction, m_normal) / t;
		double alpha = 1.0 - gamma - beta;

		if (!originEnclosed)
		{
			if (alpha >= 0.0 && beta < 0.0)
			{
				t = glm::dot((glm::dvec3)m_a, u);
				if ((gamma < 0.0) && (t > 0.0))
				{
					beta = std::min(1.0, t / glm::length2(u));
					alpha = 1.0 - beta;
					gamma = 0.0;
				}
				else
				{
					gamma = std::min(1.0, std::max(0.0, glm::dot((glm::dvec3)m_a, v) / glm::length2(v)));
					alpha = 1.0 - gamma;
					beta = 0.0;
				}
			}
			else if ((beta >= 0.0) && (gamma < 0.0))
			{
				glm::dvec3 w = m_b - m_c;
				t = glm::dot((glm::dvec3)m_b, w);
				if ((alpha < 0.0) && (t > 0.0))
				{
					gamma = std::min(1.0, t / glm::length2(w));
					beta = 1.0 - gamma;
					alpha = 0.0;
				}
				else
				{
					alpha = std::min(1.0, std::max(0.0, -glm::dot((glm::dvec3)m_b, u) / glm::length2(u)));
					beta = 1.0 - alpha;
					gamma = 0.0;
				}
			}
			else if ((gamma >= 0.0) && (alpha < 0.0))
			{
				glm::dvec3 w = m_b - m_c;
				t = -glm::dot((glm::dvec3)m_c, v);
				if ((beta < 0.0) && (t > 0.0))
				{
					alpha = std::min(1.0, t / glm::length2(v));
					gamma = 1.0 - alpha;
					beta = 0.0;
				}
				else
				{
					beta = std::min(1.0, std::max(0.0, -glm::dot((glm::dvec3)m_c, w) / glm::length2(w)));
					gamma = 1.0 - beta;
					alpha = 0.0;
				}
			}
		}

		return glm::dvec3(alpha, beta, gamma);
	}

	glm::dvec3 Triangle3D::CalcPoint(glm::dvec3 barycentric)
	{
		glm::dmat3 matrix(m_a, m_b, m_c);

		return matrix * barycentric;
	}
}
