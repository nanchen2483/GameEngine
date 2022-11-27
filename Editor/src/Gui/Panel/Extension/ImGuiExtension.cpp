#include "ImGuiExtension.h"

#include <functional>
#include <imgui.h>
#include <imgui_internal.h>

namespace Engine
{
	void ImGuiExtension::InputText(const std::string& label, std::string& text)
	{
		ImGui::Text(label.c_str());
		ImGui::SameLine(LABEL_WIDTH);
		char buffer[256];
		strcpy(buffer, text.c_str());
		if (ImGui::InputText("##InputText", buffer, IM_ARRAYSIZE(buffer)))
		{
			text = std::string(buffer);
		}
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

	void ImGuiExtension::DrawEntitySection(const std::string& label, bool isSelected, std::function<void(void)> OnSelect, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove)
	{
		ImGuiTreeNodeFlags treeNodeFlags = (isSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		bool isOpen = ImGui::TreeNodeEx((void*)std::hash<std::string>()(label), treeNodeFlags, label.c_str());
		if (ImGui::IsItemClicked())
		{
			OnSelect();
		}
		ImGui::PopStyleVar();

		bool isRemoved = false;
		if (ImGui::BeginPopup("ComponentSettings"))
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

	void ImGuiExtension::DrawPropertySection(const std::string& label, std::function<void(void)> OnOpen, std::function<void(void)> OnRemove)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
		bool isOpen = ImGui::TreeNodeEx((void*)std::hash<std::string>()(label), treeNodeFlags, label.c_str());
		ImGui::SameLine(ImGui::GetWindowContentRegionWidth() - 13.0f);
		if (ImGui::Button("+", ImVec2(20, 20)))
		{
			ImGui::OpenPopup("ComponentSettings");
		}
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

	bool ImGuiExtension::DrawFloatControl(const std::string& label, float& value, float resetValue, float speed, float min, float max)
	{
		const float originValue = value;
		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
				ImGui::Text(label.c_str());
				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::CalcItemWidth());
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.4f, 0.4f, 1.0f));
				if (ImGui::Button("V", buttonSize))
				{
					value = resetValue;
				}
				ImGui::PopStyleColor(3);

				ImGui::SameLine();
				ImGui::DragFloat("##V", &value, speed, min, max, "%.2f");

				ImGui::PopItemWidth();
				ImGui::PopStyleVar();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();

		return originValue != value;
	}

	void ImGuiExtension::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue)
	{
		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
				ImGui::Text(label.c_str());
				ImGui::NextColumn();
				{
					ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					{
						float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
						ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
							if (ImGui::Button("X", buttonSize))
							{
								values.x = resetValue;
							}
							ImGui::PopStyleColor(3);
							ImGui::SameLine();
							ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
							ImGui::PopItemWidth();
						}
						ImGui::SameLine();
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
							if (ImGui::Button("Y", buttonSize))
							{
								values.y = resetValue;
							}
							ImGui::PopStyleColor(3);

							ImGui::SameLine();
							ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
							ImGui::PopItemWidth();
						}
						ImGui::SameLine();
						{
							ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
							ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
							if (ImGui::Button("Z", buttonSize))
							{
								values.z = resetValue;
							}
							ImGui::PopStyleColor(3);

							ImGui::SameLine();
							ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
							ImGui::PopItemWidth();
						}
					}
					ImGui::PopStyleVar();
				}
			}

			ImGui::Columns(1);
		}
		ImGui::PopID();
	}
	
	void ImGuiExtension::DrawColorEdit4Control(const std::string& label, glm::vec4& color)
	{
		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
				ImGui::Text(label.c_str());
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::CalcItemWidth());
				ImGui::ColorEdit4("##label", glm::value_ptr(color), ImGuiColorEditFlags_NoLabel);
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();
	}

	void ImGuiExtension::DrawBeginComboControl(const std::string& label, std::string selected, std::unordered_map<uint32_t, std::string> options, std::function<void(uint32_t)> OnSelect)
	{
		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();
			ImGui::PushItemWidth(ImGui::CalcItemWidth());
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
			ImGui::PopItemWidth();
			ImGui::Columns(1);
		}
		ImGui::PopID();
	}

	void ImGuiExtension::DrawCheckboxControl(const std::string& label, bool* checked)
	{
		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
				ImGui::Text(label.c_str());
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::CalcItemWidth());
				ImGui::Checkbox("##Checkbox", checked);
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();
	}
	
	void ImGuiExtension::DrawTextureControl(const std::string& label, Ptr<Texture2D>& texture, TextureType type, std::function<void(void)> OnDrop)
	{
		uint64_t textureId = 0;
		std::string filePath = "";
		if (texture != nullptr)
		{
			textureId = texture->GetRendererId();
			filePath = "Path: " + texture->GetFilePath();
		}

		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
				ImGui::Text(label.c_str());

				ImGui::NextColumn();

				ImGui::PushItemWidth(ImGui::CalcItemWidth());
				ImGui::ImageButton((void*)textureId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* filepath = (const wchar_t*)payload->Data;
						const std::filesystem::path path = filepath;
						texture = TextureLibrary::GetInstance()->Load(path.string(), type);
						if (OnDrop != nullptr)
						{
							OnDrop();
						}
					}
				}
				ImGui::Text(filePath.c_str());
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();
	}
	
	void ImGuiExtension::DrawMeshControl(const std::string& label, Ptr<Model>& model, std::function<void(const std::string&)> OnDrop)
	{
		uint64_t textureId = 0;
		std::string filePath = "";
		if (model != nullptr)
		{
			filePath = model->GetFilePath().string();
		}

		ImGui::PushID(label.c_str());
		{
			ImGui::Columns(2);
			{
				ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
				ImGui::Text(label.c_str());
				ImGui::NextColumn();
				ImGui::PushItemWidth(ImGui::CalcItemWidth());
				ImGui::ImageButton((void*)textureId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* filepath = (const wchar_t*)payload->Data;
						const std::filesystem::path path = filepath;
						OnDrop(path.string());
					}
				}
				ImGui::Text(filePath.c_str());
				ImGui::PopItemWidth();
			}
			ImGui::Columns(1);
		}
		ImGui::PopID();

	}

	void ImGuiExtension::DrawAnimationControl(Ptr<Model>& model, bool& enableAnimation)
	{
		if (model != nullptr && model->HasAnimations())
		{
			ImGui::PushID("Animation");
			{
				ImGui::Columns(2);
				{
					ImGui::SetColumnWidth(0, LABEL_COLUMN_WIDTH);
					ImGui::Text("Animation");
					ImGui::NextColumn();
					ImGui::PushItemWidth(ImGui::CalcItemWidth());
					ImGui::Checkbox("##EnableAnimation", &enableAnimation);
					AnimationInfo selectedAnimation = model->GetSelectedAnimation();
					if (ImGui::BeginCombo("##BeginCombo", selectedAnimation.displayName.c_str()))
					{
						for (AnimationInfo animation : model->GetAnimations())
						{
							bool isSelected = selectedAnimation.id == animation.id;
							if (ImGui::Selectable(animation.displayName.c_str(), isSelected))
							{
								selectedAnimation = animation;
								model->SetSelectedAnimation(animation);
							}

							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					ImGui::SliderFloat("##SliderFloat", selectedAnimation.animationTime.get(), 0.0f, selectedAnimation.duration);
					ImGui::PopItemWidth();
				}
				ImGui::Columns(1);
			}
			ImGui::PopID();
		}
	}
}
