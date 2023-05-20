#pragma once
#include "IComponent.h"
#include "Engine/Physics/BoundingBox/IBoundingBox.h"

namespace Engine
{
	struct ENGINE_API PhysicsComponent : public IComponent
	{
		float mass{ 1.0f };
		float restitution{};
		bool isStatic{};
		float fallingTime{};
		Ptr<IBoundingBox> boundingBox{};

		operator Ptr<IBoundingBox>() { return boundingBox; }
	};
}
