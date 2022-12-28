#pragma once
#include "ISystem.h"
#include "Engine/Core/Base.h"
#include "Engine/Physics/Collision/Collision.h"
#include "Engine/Scene/Component/TransformComponent.h"
#include "Engine/Scene/Component/ModelComponent.h"

namespace Engine
{
	class CollisionSystem : public ISystem
	{
	public:
		static void OnUpdate(TransformComponent& transformComA, TransformComponent& transformComB, ModelComponent& modelComA, ModelComponent& modelComB);
	private:
		static Uniq<Collision> s_collision;
	};
}
