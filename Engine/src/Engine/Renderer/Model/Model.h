#pragma once
#include "Mesh.h"
#include "Animation.h"
#include "Engine/Math/Transform.h"
#include "Engine/Physics/BoundingBox/BoundingBox.h"
#include "Engine/Renderer/Camera/Frustum.h"
#include "Engine/Renderer/Texture/Texture.h"

#include <filesystem>

namespace Engine
{
	class Model : public IUniqueObject
	{
	public:
		virtual std::filesystem::path GetFilePath() = 0;
		virtual std::vector<Ptr<Mesh>> GetMeshes() const = 0;
		virtual std::vector<Ptr<Animation>> GetAnimations() const = 0;
		virtual Ptr<BoundingBox> GetBoundingBox() const = 0;
	private:
		static Ptr<Model> Create(std::string path, bool gamma = false);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId);
		static Ptr<Model> Create(std::string path, bool gamma, uint32_t entityId, Ptr<float> progression);

		friend class ModelLibrary;
	};
}
