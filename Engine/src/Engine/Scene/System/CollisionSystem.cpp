#include "CollisionSystem.h"

namespace Engine
{
	Uniq<Collision> CollisionSystem::s_collision = Collision::Create(CollisionType::GJK_EPA_3D);
	void CollisionSystem::OnUpdate(TransformComponent& transformComA, TransformComponent& transformComB, ModelComponent& modelComA, ModelComponent& modelComB)
	{
		if (modelComA.model == nullptr || modelComB.model == nullptr)
		{
			return;
		}

		s_collision->Detect(
			transformComA.transform,
			transformComB.transform,
			modelComA.model->GetBoundingValue(),
			modelComB.model->GetBoundingValue());
		if (s_collision->IsCollided())
		{
			glm::vec3 halfDistance = s_collision->GetDirectionFromAToB() / 2.0f;
			transformComA.transform.translation += halfDistance;
			transformComB.transform.translation -= halfDistance;
		}
	}
}
