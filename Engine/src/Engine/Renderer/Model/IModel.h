#pragma once
#include "IMesh.h"
#include "IAnimation.h"
#include "Engine/Math/Transform.h"
#include "Engine/Physics/BoundingBox/IBoundingBox.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Texture/ITexture.h"

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
	private:
		static Ptr<IModel> Create(std::string path);
		static Ptr<IModel> Create(std::string path, bool gamma);
		static Ptr<IModel> Create(std::string path, bool gamma, Ptr<float> progression);

		friend class ModelLibrary;
	};
}
