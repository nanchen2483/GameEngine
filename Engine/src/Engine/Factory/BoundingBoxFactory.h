#pragma once
#include "Engine/Core/Base.h"
#include "Engine/Physics/BoundingBox/IBoundingBox.h"
#include "Engine/Physics/BoundingBox/BoundingBoxType.h"

namespace Engine
{
	class ENGINE_TEST_API BoundingBoxFactory
	{
	public:
		static Ptr<IBoundingBox> Create(BoundingBoxType type, const glm::vec3& min, const glm::vec3& max);
	};
}
