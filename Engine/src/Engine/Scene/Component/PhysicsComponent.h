#pragma once
#include "IComponent.h"
#include "Engine/Physics/BoundingBox/BoundingBox.h"

namespace Engine
{
	struct PhysicsComponent : public IComponent
	{
		Ptr<BoundingBox> boundingBox;

		operator Ptr<BoundingBox>() { return boundingBox; }
	};
}
