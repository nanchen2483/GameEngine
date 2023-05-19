#pragma once
#include "Mesh/IMesh.h"
#include "Animation/IAnimation.h"
#include "Engine/Physics/BoundingBox/IBoundingBox.h"

#include <filesystem>

namespace Engine
{
	class IModel : public IUniqueObject
	{
	public:
		virtual std::filesystem::path GetFilePath() = 0;
		virtual std::vector<Ptr<IMesh>> GetMeshes() const = 0;
		virtual std::vector<Ptr<IAnimation>> GetAnimations() const = 0;
		virtual Ptr<IBoundingBox> GenerateBoundingBox() = 0;
	};
}
