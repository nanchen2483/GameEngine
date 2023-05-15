#pragma once
#include "IComponent.h"
#include "Engine/Physics/BoundingBox/IBoundingBox.h"

namespace Engine
{
	struct ENGINE_API PhysicsComponent : public IComponent
	{
		float mass = 1.0f;
		float restitution = 0.0f;
		bool isStatic = false;
		float fallingTime = 0.0f;
		Ptr<IBoundingBox> boundingBox = nullptr;

		operator Ptr<IBoundingBox>() { return boundingBox; }
	};
}
