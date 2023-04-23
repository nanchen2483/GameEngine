#include "enginepch.h"
#include "GJK3DTriangle.h"

namespace Engine
{
	GJK3DTriangle::GJK3DTriangle(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC, bool originEnclosed)
		: Triangle3D(vectorA, vectorB, vectorC)
		, m_closestPointToOrigin()
		, m_closestDistanceToOriginSquare(0.0)
		, m_isDeleted(false)
	{
		InitClosestPointToOrigin(originEnclosed);
	}

	void GJK3DTriangle::InitClosestPointToOrigin(bool originEnclosed)
	{
		if (m_normalLengthSquare > NUMERIC_EPSILON_SQUARE)
		{
			if (originEnclosed)
			{
				double delta = glm::dot(m_normal, m_vectorA);
				m_closestPointToOrigin = m_normal * (delta / m_normalLengthSquare);
			}
			else
			{
				glm::dmat3 matrix(m_vectorA, m_vectorB, m_vectorC);
				glm::dvec3 barycentric = GetBarycentric(originEnclosed);

				m_closestPointToOrigin = matrix * barycentric;
			}

			m_closestDistanceToOriginSquare = glm::length2(m_closestPointToOrigin);
		}
	}

	const glm::dvec3 GJK3DTriangle::GetBarycentric(bool originEnclosed) const
	{
		const glm::dvec3 u = m_vectorA - m_vectorB;
		const glm::dvec3 v = m_vectorA - m_vectorC;

		double denom = m_normalLengthSquare;
		double gamma = glm::dot(glm::cross(u, m_vectorA), m_normal) / denom;
		double beta = glm::dot(glm::cross(m_vectorA, v), m_normal) / denom;
		double alpha = 1.0 - gamma - beta;

		// Adjust the coordinates to ensure that they are all non-negative
		if (!originEnclosed)
		{
			if (alpha >= 0.0 && beta < 0.0)
			{
				denom = glm::dot(m_vectorA, u);
				if ((gamma < 0.0) && (denom > 0.0))
				{
					const double newBeta = std::min(1.0, denom / glm::length2(u));
					return glm::dvec3(1.0 - newBeta, newBeta, 0.0);
				}
				else
				{
					const double newGamma = glm::clamp(glm::dot(m_vectorA, v) / glm::length2(v), 0.0, 1.0);
					return glm::dvec3(1.0 - newGamma, 0.0, newGamma);
				}
			}
			else if ((beta >= 0.0) && (gamma < 0.0))
			{
				const glm::dvec3 w = m_vectorB - m_vectorC;
				denom = glm::dot(m_vectorB, w);
				if ((alpha < 0.0) && (denom > 0.0))
				{
					const double newGamma = std::min(1.0, denom / glm::length2(w));
					return glm::dvec3(0.0, 1.0 - newGamma, newGamma);
				}
				else
				{
					const double newAlpha = glm::clamp(-glm::dot(m_vectorB, u) / glm::length2(u), 0.0, 1.0);
					return glm::dvec3(newAlpha, 1.0 - newAlpha, 0.0);
				}
			}
			else if ((gamma >= 0.0) && (alpha < 0.0))
			{
				denom = -glm::dot(m_vectorC, v);
				if ((beta < 0.0) && (denom > 0.0))
				{
					const double newAlpha = std::min(1.0, denom / glm::length2(v));
					return glm::dvec3(newAlpha, 0.0, 1.0 - newAlpha);
				}
				else
				{
					const glm::dvec3 w = m_vectorB - m_vectorC;
					const double newBeta = glm::clamp(-glm::dot(m_vectorC, w) / glm::length2(w), 0.0, 1.0);
					return glm::dvec3(0.0, newBeta, 1.0 - newBeta);
				}
			}
		}

		return glm::dvec3(alpha, beta, gamma);
	}

	void GJK3DTriangle::SetLeftTriangle(const Ptr<GJK3DTriangle>& leftTriangle)
	{
		m_leftTriangle = leftTriangle;
	}
	
	void GJK3DTriangle::SetRightTriangle(const Ptr<GJK3DTriangle>& rightTriangle)
	{
		m_rightTriangle = rightTriangle;
	}
	
	void GJK3DTriangle::SetBottomTriangle(const Ptr<GJK3DTriangle>& bottomTriangle)
	{
		m_bottomTriangle = bottomTriangle;
	}
	
	void GJK3DTriangle::SetNeighbors(const Ptr<GJK3DTriangle>& leftTriangle, const Ptr<GJK3DTriangle>& rightTriangle, const Ptr<GJK3DTriangle>& bottomTriangle)
	{
		m_leftTriangle = leftTriangle;
		m_rightTriangle = rightTriangle;
		m_bottomTriangle = bottomTriangle;
	}

	bool GJK3DTriangle::IsExpandable(const glm::dvec3& point) const
	{
		return glm::dot(point - m_vectorA, m_normal) > 0.0;
	}
	
	void GJK3DTriangle::MarkAsDeleted()
	{
		m_isDeleted = true;
	}

	bool GJK3DTriangle::operator==(const GJK3DTriangle& other) const
	{
		return this == &other;
	}

	bool GJK3DTriangle::operator!=(const GJK3DTriangle& other) const
	{
		return this != &other;
	}

	bool GJK3DTriangle::operator<(const GJK3DTriangle& other) const
	{
		return this->m_closestDistanceToOriginSquare < other.m_closestDistanceToOriginSquare;
	}

	bool GJK3DTriangle::operator<=(const GJK3DTriangle& other) const
	{
		return this->m_closestDistanceToOriginSquare <= other.m_closestDistanceToOriginSquare;
	}

	bool GJK3DTriangle::operator>(const GJK3DTriangle& other) const
	{
		return  this->m_closestDistanceToOriginSquare > other.m_closestDistanceToOriginSquare;
	}

	bool GJK3DTriangle::operator>=(const GJK3DTriangle& other) const
	{
		return  this->m_closestDistanceToOriginSquare >= other.m_closestDistanceToOriginSquare;
	}
}
