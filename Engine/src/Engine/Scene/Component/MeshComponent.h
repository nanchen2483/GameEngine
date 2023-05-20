#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Mesh/IMesh.h"

#include <string>

namespace Engine
{
	struct ENGINE_API MeshComponent : public IComponent
	{
		bool isLoading{};
		bool isPlayer{};
		bool isOnViewFrustum{};
		bool isOnLightViewFrustum{};
		std::string filePath{};
		std::vector<Ptr<IMesh>> meshes{};

		operator std::vector<Ptr<IMesh>>() { return meshes; }
	};
}
