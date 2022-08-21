#pragma once
#include <glm/glm.hpp>

namespace Engine
{
	struct Vertex3D
	{
		int32_t id;
		glm::dvec3 value;
		
		double x() const { return value.x; }
		double y() const { return value.y; }
		double z() const { return value.z; }

		operator glm::dvec3() const { return value; }

		glm::dvec3 operator+(const Vertex3D& rhs)	{ return this->value + rhs.value; }
		glm::dvec3 operator+(const glm::dvec3& rhs) { return this->value + rhs; }
		glm::dvec3 operator-(const Vertex3D& rhs)	{ return this->value - rhs.value; }
		glm::dvec3 operator-(const glm::dvec3& rhs) { return this->value - rhs; }
		glm::dvec3 operator*(const Vertex3D& rhs) { return this->value * rhs.value; }
		glm::dvec3 operator*(const glm::dvec3& rhs) { return this->value * rhs; }
		glm::dvec3 operator/(const Vertex3D& rhs) { return this->value / rhs.value; }
		glm::dvec3 operator/(const glm::dvec3& rhs) { return this->value / rhs; }
	};
}
