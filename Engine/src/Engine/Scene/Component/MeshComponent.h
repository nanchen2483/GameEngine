#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Mesh.h"

#include <string>

namespace Engine
{
	struct MeshComponent : public IComponent
	{
		bool isLoading = false;
		bool isPlayer = false;
		bool isOnViewFrustum = false;
		std::string filePath;
		std::vector<Ptr<Mesh>> meshes;

		operator std::vector<Ptr<Mesh>>() { return meshes; }
	};
}
