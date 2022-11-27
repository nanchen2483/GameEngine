#pragma once
#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>
#include <string>

#define LABEL_COLUMN_WIDTH 100.0f
#define LABEL_WIDTH 50.0f

namespace Engine
{
	class ImGuiExtension
	{
	public:
		static void InputText(const std::string& label, std::string& text);
		static void LabelText(const std::string& label, const std::string& text);
		static void ProgressBar(float fragtion);

		static void DrawEntitySection(const std::string& label, bool isSelected, std::function<void(void)> OnSelect, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove);
		static void DrawPropertySection(const std::string& label, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove);

		static bool DrawFloatControl(const std::string& label, float& value, float resetValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f);
		static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f);
		static void DrawColorEdit4Control(const std::string& label, glm::vec4& color);
		static void DrawBeginComboControl(const std::string& label, std::string selected, std::unordered_map<uint32_t, std::string> options, std::function<void(uint32_t)> OnSelect);
		static void DrawCheckboxControl(const std::string& label, bool *checked);
		
		static void DrawTextureControl(const std::string& label, Ptr<Texture2D>& texture, TextureType type = TextureType::Diffuse, std::function<void(void)> OnDrop = nullptr);
		static void DrawMeshControl(const std::string& label, Ptr<Model>& model, std::function<void(const std::string&)> OnDrop);
		static void DrawAnimationControl(Ptr<Model>& model, bool& enableAnimation);
	};
}
