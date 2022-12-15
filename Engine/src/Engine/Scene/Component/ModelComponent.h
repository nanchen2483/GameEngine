#pragma once
#include "IComponent.h"
#include "Engine/Renderer/Model/Model.h"

namespace Engine
{
	struct ModelComponent : public IComponent
	{
		bool isPlayer = false;
		bool loading = false;
		Ptr<float> progression = CreatePtr<float>(0.0f);
		bool isOnViewFrustum = false;

		bool enableAnimation = false;
		Ptr<Model> model = nullptr;

		void OnUpdate(const Frustum& frustum, const Transform& transform)
		{
			bool isOnViewFrustum = IsOnViewFrustum(frustum, transform);
			if (isOnViewFrustum && enableAnimation && model != nullptr)
			{
				model->OnUpdate();
			}
		}
	private:
		bool IsOnViewFrustum(const Frustum& frustum, const Transform& transform)
		{
			isOnViewFrustum = false;
			if (model != nullptr)
			{
				isOnViewFrustum = model->IsOnFrustum(frustum, transform);
			}

			return isOnViewFrustum;
		}
	};
}
