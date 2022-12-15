#pragma once
#include "IComponent.h"
#include "TransformComponent.h"
#include "Engine/Renderer/Terrain/Terrain.h"

namespace Engine
{
	struct TerrainComponent : public IComponent
	{
		Ptr<Terrain> terrain = nullptr;
		Ptr<Texture2D> texture = nullptr;

		void SetHeight(TransformComponent& component)
		{
			if (terrain != nullptr)
			{
				component.transform.translation.y = terrain->GetHeight(component.transform.translation.x, component.transform.translation.z);
			}
		}

		void OnUpdate(glm::vec3 cameraPosition)
		{
			if (terrain != nullptr)
			{
				terrain->OnUpdate(cameraPosition);
			}
		}
	};
}
