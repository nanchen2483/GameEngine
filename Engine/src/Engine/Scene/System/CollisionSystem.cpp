#include "enginepch.h"
#include "CollisionSystem.h"

namespace Engine
{
	Uniq<Collision> CollisionSystem::s_collision = Collision::Create(CollisionType::GJK_EPA_3D);
	void CollisionSystem::OnUpdate(TransformComponent& transformComA, TransformComponent& transformComB, ModelComponent& modelComA, ModelComponent& modelComB)
	{
		if (modelComA.boundingBox == nullptr || modelComB.boundingBox == nullptr)
		{
			return;
		}

		s_collision->Detect(
			transformComA.transform,
			transformComB.transform,
			modelComA.boundingBox->GetBoundingValue(),
			modelComB.boundingBox->GetBoundingValue());
		if (s_collision->IsCollided())
		{
			glm::vec3 halfDistance = s_collision->GetDirectionFromAToB() / 2.0f;
			transformComA.transform.translation += halfDistance;
			transformComB.transform.translation -= halfDistance;
		}
	}
}
