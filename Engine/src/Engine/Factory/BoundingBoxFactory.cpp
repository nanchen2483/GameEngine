#include "enginepch.h"
#include "BoundingBoxFactory.h"

#include "Engine/Physics/BoundingBox/AABB/AABB.h"

namespace Engine
{
	Ptr<IBoundingBox> BoundingBoxFactory::Create(BoundingBoxType type, const glm::vec3& min, const glm::vec3& max)
	{
		switch (type)
		{
		case Engine::BoundingBoxType::AABB:
			return CreatePtr<AABB>(min, max);
		default:
			ENGINE_CORE_ASSERT(false, "Unknow Bouding Box type");
			return nullptr;
		}
	}
}
