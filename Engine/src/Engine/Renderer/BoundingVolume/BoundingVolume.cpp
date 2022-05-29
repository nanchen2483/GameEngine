#include "enginepch.h"
#include "BoundingVolume.h"
#include "AABB.h"

namespace Engine
{
	Uniq<BoundingVolume> BoundingVolume::Create(BoundingVolumeType type, const glm::vec3& min, const glm::vec3& max)
	{
		switch (type)
		{
		case Engine::BoundingVolumeType::AABB:
			return CreateUniq<AABB>(min, max);
		case Engine::BoundingVolumeType::Unknown:
		default:
			ENGINE_CORE_ASSERT(false, "Unknow Bouding Volume type");
			return nullptr;
		}
	}
}
