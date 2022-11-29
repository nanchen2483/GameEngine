#pragma once
#include "Engine.h"

#include <glm/gtc/type_ptr.hpp>
#include <string>

#define LABEL_COLUMN_WIDTH 100.0f
#define LABEL_WIDTH 50.0f

namespace Engine
{
	enum class ImGuiColor
	{
		Default = 0,
		Red = 1,
		Green,
		Blue,
	};

	class ImGuiExtension
	{
	public:
		static void InputText(const std::string& label, std::string& text);
		static void LabelText(const std::string& label, const std::string& text);
		static void ProgressBar(float fragtion);

		static void DrawEntitySection(const std::string& label, bool isSelected, std::function<void(void)> OnSelect, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove);
		static void DrawPropertySection(const std::string& label, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove);

		static bool DrawFloatSubSection(const std::string& label, float& value, float resetValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f);
		static void DrawVec3SubSection(const std::string& label, glm::vec3& values, float resetValue = 0.0f);
		static void DrawColorEdit4SubSection(const std::string& label, glm::vec4& color);
		static void DrawBeginComboSubSection(const std::string& label, std::string selected, std::unordered_map<uint32_t, std::string> options, std::function<void(uint32_t)> OnSelect);
		static void DrawCheckboxSubSection(const std::string& label, bool *checked);
		
		static void DrawTextureSubSection(const std::string& label, Ptr<Texture2D>& texture, TextureType type = TextureType::Diffuse, std::function<void(void)> OnDrop = nullptr);
		static void DrawMeshSubSection(const std::string& label, Ptr<Model>& model, std::function<void(const std::string&)> OnDrop);
		static void DrawAnimationSubSection(Ptr<Model>& model, bool& enableAnimation);

		static bool DrawFloatControl(const std::string& label, float& value, float resetValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f, ImGuiColor buttonColor = ImGuiColor::Blue);
		static void ButtonColor(ImGuiColor buttonColor, std::function<void(void)> InlineCode);
	};
}
