#pragma once
#include "IComponent.h"
#include "Engine/Physics/BoundingBox/BoundingBox.h"

namespace Engine
{
	struct PhysicsComponent : public IComponent
	{
		float weight = 1.0f;
		bool isStatic = false;
		float fallingTime = 0.0f;
		Ptr<BoundingBox> boundingBox = nullptr;

		operator Ptr<BoundingBox>() { return boundingBox; }
	};
}
