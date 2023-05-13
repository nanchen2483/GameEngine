#pragma once
#include "Scene.h"

namespace Engine
{
	class SceneSerializer
	{
	public:
		static void Serialize(Scene* scene, const std::string& filepath);
		static void SerializeRuntime(Scene* scene, const std::string& filepath);

		static bool Deserialize(Scene* scene, const std::string& filepath);
		static bool DeserializeRuntime(Scene* scene, const std::string& filepath);
	};
}
