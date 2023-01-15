#include "enginepch.h"
#include "CollisionSystem.h"

namespace Engine
{
	Uniq<Collision> CollisionSystem::s_collision = Collision::Create(CollisionType::GJK_EPA_3D);
	void CollisionSystem::OnUpdate(Transform& transformA, Transform& transformB, Ptr<BoundingBox> boundingBoxA, Ptr<BoundingBox> boundingBoxB)
	{
		if (boundingBoxA == nullptr || boundingBoxB == nullptr)
		{
			return;
		}

		s_collision->Detect(
			transformA,
			transformB,
			boundingBoxA->GetBoundingValue(),
			boundingBoxB->GetBoundingValue());
		if (s_collision->IsCollided())
		{
			glm::vec3 halfDistance = s_collision->GetDirectionFromAToB() / 2.0f;
			transformA.translation += halfDistance;
			transformB.translation -= halfDistance;
		}
	}
}
