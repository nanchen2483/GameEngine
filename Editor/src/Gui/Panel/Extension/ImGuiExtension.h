#pragma once
#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>
#include <string>

#define LABEL_COLUMN_WIDTH 100.0f

namespace Engine
{
	class ImGuiExtension
	{
	public:
		static void InputText(const std::string& label, std::string& text);
		static void DrawSection(const std::string& name, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove);
		static void DrawFloatControl(const std::string& label, float& value, float resetValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f);
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float lableColumnWidth = LABEL_COLUMN_WIDTH);
		static void DrawColorEdit4Control(const std::string& label, glm::vec4& color, float lableColumnWidth = LABEL_COLUMN_WIDTH);
		static void DrawTextureControl(const std::string& label, Ptr<Texture2D>& texture, TextureType type = TextureType::Diffuse, float lableColumnWidth = LABEL_COLUMN_WIDTH);
	};
}
