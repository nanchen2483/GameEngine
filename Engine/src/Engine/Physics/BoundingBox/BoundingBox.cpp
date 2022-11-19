#include "enginepch.h"
#include "BoundingBox.h"

#include "AABB.h"

namespace Engine
{
	Uniq<BoundingBox> BoundingBox::Create(BoundingBoxType type, const glm::vec3& min, const glm::vec3& max)
	{
		switch (type)
		{
		case Engine::BoundingBoxType::AABB:
			return CreateUniq<AABB>(min, max);
		default:
			ENGINE_CORE_ASSERT(false, "Unknow Bouding Box type");
			return nullptr;
		}
	}
}
