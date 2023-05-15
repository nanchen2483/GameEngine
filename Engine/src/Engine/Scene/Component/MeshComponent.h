#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/IMesh.h"

#include <string>

namespace Engine
{
	struct ENGINE_API MeshComponent : public IComponent
	{
		bool isLoading = false;
		bool isPlayer = false;
		bool isOnViewFrustum = false;
		bool isOnLightViewFrustum = false;
		std::string filePath;
		std::vector<Ptr<IMesh>> meshes;

		operator std::vector<Ptr<IMesh>>() { return meshes; }
	};
}
