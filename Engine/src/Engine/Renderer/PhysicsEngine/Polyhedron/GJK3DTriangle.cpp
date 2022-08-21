#include "enginepch.h"
#include "GJK3DTriangle.h"

namespace Engine
{
	GJK3DTriangle::GJK3DTriangle(const Vertex3D& a, const Vertex3D& b, const Vertex3D& c)
		: Triangle3D(a, b, c)
	{
	}

	void GJK3DTriangle::SetLeftTriangle(const uint32_t leftTriangle)
	{
		m_leftTriangle = leftTriangle;
	}
	
	void GJK3DTriangle::SetRightTriangle(const uint32_t rightTriangle)
	{
		m_rightTriangle = rightTriangle;
	}
	
	void GJK3DTriangle::SetBottomTriangle(const uint32_t bottomTriangle)
	{
		m_bottomTriangle = bottomTriangle;
	}
	
	void GJK3DTriangle::SetNeighbors(const uint32_t leftTriangle, const uint32_t rightTriangle, const uint32_t bottomTriangle)
	{
		m_leftTriangle = leftTriangle;
		m_rightTriangle = rightTriangle;
		m_bottomTriangle = bottomTriangle;
	}
	
	void GJK3DTriangle::SetPreviousTriangle(const uint32_t previousTriangle)
	{
		m_previousTriangle = previousTriangle;
	}
	
	void GJK3DTriangle::SetNextTriangle(const uint32_t nextTriangle)
	{
		m_nextTriangle = nextTriangle;
	}
	
	void GJK3DTriangle::MarkAsDeleted()
	{
		m_isDeleted = true;
	}
}
