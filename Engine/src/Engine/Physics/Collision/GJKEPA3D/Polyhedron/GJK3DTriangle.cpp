#include "enginepch.h"
#include "GJK3DTriangle.h"

namespace Engine
{
	GJK3DTriangle::GJK3DTriangle(const glm::dvec3& vectorA, const glm::dvec3& vectorB, const glm::dvec3& vectorC)
		: Triangle3D(vectorA, vectorB, vectorC)
	{
		m_leftTriangle = nullptr;
		m_rightTriangle = nullptr;
		m_bottomTriangle = nullptr;
		m_isDeleted = false;
	}

	void GJK3DTriangle::SetLeftTriangle(GJK3DTriangle* const& leftTriangle)
	{
		m_leftTriangle = leftTriangle;
	}
	
	void GJK3DTriangle::SetRightTriangle(GJK3DTriangle* const& rightTriangle)
	{
		m_rightTriangle = rightTriangle;
	}
	
	void GJK3DTriangle::SetBottomTriangle(GJK3DTriangle* const& bottomTriangle)
	{
		m_bottomTriangle = bottomTriangle;
	}
	
	void GJK3DTriangle::SetNeighbors(GJK3DTriangle* const& leftTriangle, GJK3DTriangle* const& rightTriangle, GJK3DTriangle* const& bottomTriangle)
	{
		m_leftTriangle = leftTriangle;
		m_rightTriangle = rightTriangle;
		m_bottomTriangle = bottomTriangle;
	}

	bool GJK3DTriangle::IsExpandable(const glm::dvec3& point) const
	{
		glm::dvec3 direction = point - m_vectorA;

		return glm::dot(direction, m_normal) > 0;
	}
	
	void GJK3DTriangle::MarkAsDeleted()
	{
		m_isDeleted = true;
	}

	bool GJK3DTriangle::operator==(const GJK3DTriangle& other) const
	{
		return this->GetClosestDistanceToOriginSquare() == other.GetClosestDistanceToOriginSquare();
	}

	bool GJK3DTriangle::operator!=(const GJK3DTriangle& other) const
	{
		return this->GetClosestDistanceToOriginSquare() != other.GetClosestDistanceToOriginSquare();
	}

	bool GJK3DTriangle::operator<(const GJK3DTriangle& other) const
	{
		return this->GetClosestDistanceToOriginSquare() < other.GetClosestDistanceToOriginSquare();
	}

	bool GJK3DTriangle::operator<=(const GJK3DTriangle& other) const
	{
		return this->GetClosestDistanceToOriginSquare() <= other.GetClosestDistanceToOriginSquare();
	}

	bool GJK3DTriangle::operator>(const GJK3DTriangle& other) const
	{
		return  this->GetClosestDistanceToOriginSquare() > other.GetClosestDistanceToOriginSquare();
	}

	bool GJK3DTriangle::operator>=(const GJK3DTriangle& other) const
	{
		return  this->GetClosestDistanceToOriginSquare() >= other.GetClosestDistanceToOriginSquare();
	}
}
