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

		void SetHeight(TransformComponent& translate)
		{
			if (terrain != nullptr)
			{
				translate.transform.translation.y = terrain->GetHeight(translate.GetTranslation().x, translate.GetTranslation().z);
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
