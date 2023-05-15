#include "enginepch.h"
#include "IBoundingBox.h"

#include "AABB.h"

namespace Engine
{
	Ptr<IBoundingBox> IBoundingBox::Create(BoundingBoxType type, const glm::vec3& min, const glm::vec3& max)
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
