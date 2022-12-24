#pragma once
#include "IComponent.h"
#include "Engine/Scene/ScriptableEntity.h"

namespace Engine
{
	struct NativeScriptComponent : public IComponent
	{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent* nsc);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};
}