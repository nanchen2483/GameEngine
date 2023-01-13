#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Model.h"
#include "Engine/Renderer/Model/Mesh.h"
#include "Engine/Renderer/Model/Animation.h"

namespace Engine
{
	struct ModelComponent : public IComponent
	{
		std::string filePath;

		bool isPlayer = false;
		bool loading = false;
		Ptr<float> progression = CreatePtr<float>(0.0f);
		bool isOnViewFrustum = false;
		bool enableAnimation = false;
		uint32_t selectedAnimationIndex;

		std::vector<Ptr<Mesh>> meshes;
		std::vector<Ptr<Animation>> animations;
		Ptr<BoundingBox> boundingBox;
	};
}
