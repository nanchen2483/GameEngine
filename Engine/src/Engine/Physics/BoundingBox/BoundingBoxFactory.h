#pragma once
#include "IBoundingBox.h"
#include "BoundingBoxType.h"
#include "Engine/Core/Base.h"

namespace Engine
{
	class ENGINE_TEST_API BoundingBoxFactory
	{
	public:
		static Ptr<IBoundingBox> Create(BoundingBoxType type, const glm::vec3& min, const glm::vec3& max);
	};
}
