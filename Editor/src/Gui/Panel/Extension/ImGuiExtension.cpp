#include "ImGuiExtension.h"

#include <functional>
#include <imgui.h>
#include <imgui_internal.h>
#include <IconFontCppHeaders/IconsFontAwesome6.h>

namespace Engine
{
	void ImGuiExtension::EditableText(const std::string& label, std::string& text, float labelSize)
	{
		static bool isEditMode = false;
		ImGui::Text(label.c_str());
		ImGui::SameLine(labelSize == -1 ? LABEL_WIDTH : 0);
		if (isEditMode)
		{
			char buffer[256];
			strcpy(buffer, text.c_str());
			ImGui::PushItemWidth(-1);
			if (ImGui::InputText("##EditableText", buffer, IM_ARRAYSIZE(buffer)))
			{
				text = std::string(buffer);
			}
			ImGui::PopItemWidth();
			if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				isEditMode = false;
			}
		}
		else
		{
			ImGui::Text(text.c_str());
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				isEditMode = true;
			
			}
		}
	}

	void ImGuiExtension::InputText(const std::string& label, std::string& text)
	{
		ImGui::Text(label.c_str());
		ImGui::SameLine(LABEL_WIDTH);
		char buffer[256];
		strcpy(buffer, text.c_str());
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##InputText", buffer, IM_ARRAYSIZE(buffer)))
		{
			text = std::string(buffer);
		}
		ImGui::PopItemWidth();
	}

	void ImGuiExtension::LabelText(const std::string& label, const std::string& text)
	{
		ImGui::Text(label.c_str());
		ImGui::SameLine(LABEL_WIDTH);
		ImGui::LabelText("##LabelText", text.c_str());
	}

	void ImGuiExtension::ProgressBar(float fragtion)
	{
		ImGui::ProgressBar(fragtion, ImVec2(0.0f, 0.0f));
		ImGui::Text("Loading...");
	}

	void ImGuiExtension::DrawSection(uint32_t id, const std::string& label, int treeNodeFlags, std::function<void(void)> InlineCode, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		bool isOpen = ImGui::TreeNodeEx((void*)id, (ImGuiTreeNodeFlags)treeNodeFlags | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth, label.c_str());
		InlineCode();
		ImGui::PopStyleVar();

		bool isRemoved = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
			{
				isRemoved = true;
			}
			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				isRemoved = true;
			}
			ImGui::EndPopup();
		}

		if (isOpen)
		{
			OnOpen();
			ImGui::TreePop();
		}

		if (isRemoved)
		{
			OnRemove();
		}
	}

	void ImGuiExtension::DrawEntitySection(uint32_t id, const std::string& label, bool isSelected, std::function<void(void)> OnSelect, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove)
	{
		ImGuiTreeNodeFlags treeNodeFlags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		DrawSection(id, label, treeNodeFlags,
			[&]()
			{
				if (ImGui::IsItemClicked())
				{
					OnSelect();
				}
			},
			[&]() { OnOpen(); },
			[&]() { OnRemove(); });
	}

	void ImGuiExtension::DrawPropertySection(uint32_t id, const std::string& label, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog;
		DrawSection(id, label, treeNodeFlags,
			[&]()
			{
				ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 8.0f);
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::Button("+", ImVec2(20, 20)))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
				ImGui::PopStyleColor();
			},
			[&]() { OnOpen(); },
			[&]() { OnRemove(); });
	}

	void ImGuiExtension::DrawPropertySubSection(const std::string& label, std::function<void(void)> InlineCode)
	{
		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				float labelWidth = LABEL_COLUMN_WIDTH;
				float currentRowTotalWidth = ImGui::GetWindowContentRegionWidth();
				if (currentRowTotalWidth < (LABEL_COLUMN_WIDTH + VALUE_COLUMN_MINIMUM_WIDTH))
				{
					labelWidth = currentRowTotalWidth - VALUE_COLUMN_MINIMUM_WIDTH;
					if (labelWidth < LABEL_COLUMN_MINIMUM_WIDTH)
					{
						labelWidth = -1;
					}
				}

				ImGui::SetColumnWidth(0, labelWidth);
				ImGui::Text(label.c_str());
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				InlineCode();
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();
	}

	void ImGuiExtension::DrawPropertyTagSection(std::string& tag, std::string buttonLabel, glm::vec2 buttonSize, std::function<void(void)> OnClick)
	{
		ImGui::PushID(ICON_FA_TAG);
		{
			ImGui::Columns(2, 0, false);
			{
				ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() - buttonSize.x);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + buttonSize.y / 2 - ImGui::GetFontSize() / 2);
				EditableText(ICON_FA_TAG, tag, 0);
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				if (ImGui::Button(buttonLabel.c_str(), { buttonSize.x, buttonSize.y }))
				{
					ImGui::OpenPopup("AddComponent");
				}

				if (ImGui::BeginPopup("AddComponent"))
				{
					OnClick();
					ImGui::EndPopup();
				}
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();
	}

	bool ImGuiExtension::DrawFloatSubSection(const std::string& label, float& value, float resetValue, float speed, float min, float max)
	{
		bool isChanged = false;
		DrawPropertySubSection(label,
			[&]()
			{
				isChanged = DrawFloatControl("V", value, resetValue, speed, min, max, ImGuiColor::Default);
			});
		return isChanged;
	}

	void ImGuiExtension::DrawVec3SubSection(const std::string& label, glm::vec3& values, float resetValue)
	{
		DrawPropertySubSection(label,
			[&]()
			{
				ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth() - (5 + 2) * 3);
				{
					DrawFloatControl("X", values.x, resetValue, 0.1f, 0.0f, 0.0f, ImGuiColor::Red);
					ImGui::PopItemWidth();
				}
				ImGui::SameLine();
				{
					DrawFloatControl("Y", values.y, resetValue, 0.1f, 0.0f, 0.0f, ImGuiColor::Green);
					ImGui::PopItemWidth();
				}
				ImGui::SameLine();
				{
					DrawFloatControl("Z", values.z, resetValue, 0.1f, 0.0f, 0.0f, ImGuiColor::Blue);
					ImGui::PopItemWidth();
				}
			});
	}
	
	void ImGuiExtension::DrawColorEdit4SubSection(const std::string& label, glm::vec4& color)
	{
		DrawPropertySubSection(label,
			[&]()
			{
				ImGui::ColorEdit4("##label", glm::value_ptr(color), ImGuiColorEditFlags_NoLabel);
			});
	}

	void ImGuiExtension::DrawBeginComboSubSection(const std::string& label, std::string selected, std::unordered_map<uint32_t, std::string> options, std::function<void(uint32_t)> OnSelect)
	{
		DrawPropertySubSection(label,
			[&]()
			{
				if (ImGui::BeginCombo("##BeginCombo", selected.c_str()))
				{
					for (std::pair<uint32_t, std::string> option : options)
					{
						bool isSelected = option.second == selected;
						if (ImGui::Selectable(option.second.c_str(), isSelected))
						{
							OnSelect(option.first);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			});
	}

	void ImGuiExtension::DrawCheckboxSubSection(const std::string& label, bool* checked)
	{
		DrawPropertySubSection(label,
			[&]()
			{
				ImGui::Checkbox("##Checkbox", checked);
			});
	}
	
	void ImGuiExtension::DrawTextureSubSection(const std::string& label, Ptr<Texture2D>& texture, TextureType type, std::function<void(void)> OnDrop)
	{
		uint64_t textureId = 0;
		std::string filePath = "none";
		if (texture != nullptr)
		{
			textureId = texture->GetRendererId();
			filePath = texture->GetFilePath();
		}

		DrawPropertySubSection(label,
			[&]()
			{
				ImGui::ImageButton((void*)textureId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(FileType::Texture.c_str()))
					{
						const wchar_t* filepath = (const wchar_t*)payload->Data;
						const std::filesystem::path path = filepath;
						texture = TextureLibrary::GetInstance()->Load(path.string(), type);
						if (OnDrop != nullptr)
						{
							OnDrop();
						}
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::Text(filePath.c_str());
			});
	}
	
	void ImGuiExtension::DrawMeshSubSection(const std::string& label, const std::string& filePath, std::function<void(const std::string&)> OnDrop)
	{
		uint64_t textureId = 0;
		DrawPropertySubSection(label,
			[&]()
			{
				ImGui::ImageButton((void*)textureId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(FileType::Model.c_str()))
					{
						const wchar_t* filepath = (const wchar_t*)payload->Data;
						const std::filesystem::path path = filepath;
						OnDrop(path.string());
					}
					ImGui::EndDragDropTarget();
				}

				if (filePath.empty())
				{
					ImGui::Text("none");
				}
				else
				{
					ImGui::Text(filePath.c_str());
				}
			});

	}

	void ImGuiExtension::DrawAnimationSubSection(std::vector<Ptr<Animation>> animations, uint32_t selectedAnimationIndex, bool& isEnabled, std::function<void(uint32_t)> OnSelect)
	{
		if (!animations.empty())
		{
			DrawPropertySubSection("Animation",
				[&]()
				{
					ImGui::Checkbox("##EnableAnimation", &isEnabled);
					Ptr<Animation> selectedAnimation = animations[selectedAnimationIndex];
					if (ImGui::BeginCombo("##BeginCombo", selectedAnimation->GetName().c_str()))
					{
						for (uint32_t index = 0; index < animations.size(); index++)
						{
							Ptr<Animation> currentAnimation = animations[index];
							bool isSelected = selectedAnimationIndex == index;
							if (ImGui::Selectable(currentAnimation->GetName().c_str(), isSelected))
							{
								selectedAnimationIndex = index;
								OnSelect(index);
							}

							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SliderFloat("##SliderFloat", selectedAnimation->GetTime().get(), 0.0f, selectedAnimation->GetDuration());
				});
		}
	}
	
	bool ImGuiExtension::DrawFloatControl(const std::string& buttonLabel, float& value, float resetValue, float speed, float min, float max, ImGuiColor buttonColor)
	{
		const float originValue = value;
		ButtonColor(buttonColor,
			[&]()
			{
				if (ImGui::Button(("##DrawFloatControlButton" + buttonLabel).c_str(), {5 , 0}))
				{
					value = resetValue;
				}
				ImGui::SameLine(0, 0);
				ImGui::DragFloat(("##DrawFloatControlDragFloat" + buttonLabel).c_str(), &value, speed, min, max, "%.2f");
			});
		return originValue != value;
	}

	void ImGuiExtension::ButtonColor(ImGuiColor buttonColor, std::function<void(void)> InlineCode)
	{
		switch (buttonColor)
		{
		case Engine::ImGuiColor::Red:
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
			break;
		case Engine::ImGuiColor::Green:
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.9f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.8f, 0.1f, 1.0f));
			break;
		case Engine::ImGuiColor::Blue:
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.8f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.9f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.8f, 1.0f));
			break;
		case Engine::ImGuiColor::Default:
		default:
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.3f, 0.7f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.8f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.7f, 1.0f));
			break;
		}

		InlineCode();
		ImGui::PopStyleColor(3);
	}
}
