#include "SceneHierarchyPanel.h"

#include "Engine/Scene/Component.h"

#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <thread>

namespace Engine
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ptr<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ptr<Scene>& context)
	{
		m_context = context;
		m_selectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_context->m_registry.each([&](entt::entity entityId) {
			Entity entity = Entity{ entityId, m_context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectionContext = {};
		}

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_context->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selectionContext)
		{
			DrawComponents(m_selectionContext);
			
			ImGui::NewLine();
			ImGui::Separator();
			ImGui::NewLine();
			
			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!m_selectionContext.HasComponent<CameraComponent>() && ImGui::MenuItem("Camera"))
				{
					m_selectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!m_selectionContext.HasComponent<LightComponent>() && ImGui::MenuItem("Light"))
				{
					m_selectionContext.AddComponent<LightComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!m_selectionContext.HasComponent<SpriteRendererComponent>() && ImGui::MenuItem("Sprite Renderer"))
				{
					m_selectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!m_selectionContext.HasComponent<ModelComponent>() && ImGui::MenuItem("Model"))
				{
					m_selectionContext.AddComponent<ModelComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!m_selectionContext.HasComponent<SkyboxComponent>() && ImGui::MenuItem("Skybox"))
				{
					m_selectionContext.AddComponent<SkyboxComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (!m_selectionContext.HasComponent<TerrainComponent>() && ImGui::MenuItem("Terrian"))
				{
					m_selectionContext.AddComponent<TerrainComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}
	
	void SceneHierarchyPanel::SetSelectedEntity(Entity& entity)
	{
		m_selectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_context->DestoryEntity(entity);
			if (m_selectionContext == entity)
			{
				m_selectionContext = {};
			}
		}
	}

	static void DrawFloatControl(const std::string& label, float& value, float resetValue = 0.0f, float speed = 0.1f, float min = 0.0f, float max = 0.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
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

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

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
		ImGui::SameLine();

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
		ImGui::SameLine();

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

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	namespace Enum
	{
		static const char* ToString(TextureOrientationType o)
		{
			switch (o)
			{
			case TextureOrientationType::None:
				return nullptr;
			case TextureOrientationType::Right:
				return "Right";
			case TextureOrientationType::Left:
				return "Left";
			case TextureOrientationType::Top:
				return "Top";
			case TextureOrientationType::Bottom:
				return "Bottom";
			case TextureOrientationType::Back:
				return "Back";
			case TextureOrientationType::Front:
				return "Front";
			default:
				ENGINE_CORE_ERROR("Unsupported Orientation type");
				return nullptr;
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, IM_ARRAYSIZE(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			ImGui::SameLine(ImGui::GetWindowWidth() - 35.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				Transform& transform = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", transform.translation);
				glm::vec3 rotation = glm::degrees(transform.rotation);
				DrawVec3Control("Rotation", rotation);
				transform.rotation = glm::radians(rotation);
				DrawVec3Control("Scale", transform.scale, 1.0f);

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_selectionContext.RemoveComponent<TransformComponent>();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Camera"))
			{
				CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
				SceneCamera& camera = cameraComponent.camera;

				ImGui::Checkbox("Primary", &cameraComponent.primary);

				std::string selectedProjectionString = camera.GetProjectionTypeString();
				if (ImGui::BeginCombo("Projection", selectedProjectionString.c_str()))
				{
					for (auto& it : camera.GetProjectionTypeMap())
					{
						SceneCamera::ProjectionType currentProjectionType = it.first;
						std::string currentProjectionString = it.second;

						bool isSelected = selectedProjectionString == currentProjectionString;
						if (ImGui::Selectable(currentProjectionString.c_str(), isSelected))
						{
							selectedProjectionString = currentProjectionString;
							camera.SetProjectionType(currentProjectionType);
						}

						if (isSelected) {
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
				
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float FOV = camera.GetPerspectiveFOV();
					if (ImGui::DragFloat("FOV", &FOV))
					{
						camera.SetPerspectiveFOV(FOV);
					}

					float perspectNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectNear))
					{
						camera.SetPerspectiveNearClip(perspectNear);
					}

					float perspectFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectFar))
					{
						camera.SetPerspectiveFarClip(perspectFar);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
					{
						camera.SetOrthographicNearClip(orthoNear);
					}

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
					{
						camera.SetOrthographicFarClip(orthoFar);
					}

					ImGui::Checkbox("Fixed aspect ratio", &cameraComponent.fixedAspectRatio);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<LightComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(LightComponent).hash_code(), treeNodeFlags, "Light");
			ImGui::SameLine(ImGui::GetWindowWidth() - 35.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				LightComponent& component = entity.GetComponent<LightComponent>();
				DrawFloatControl("Constant", component.constant, 1.0f, 0.01f, 0.0f, 1.0f);
				DrawFloatControl("Linear", component.linear, 0.9f, 0.001f, 0.001f, 1.0f);
				DrawFloatControl("Quadratic", component.quadratic, 0.03f, 0.001f, 0.01f, 2.0f);
				ImGui::NewLine();
				DrawVec3Control("Ambient", component.ambient);
				DrawVec3Control("Diffuse", component.diffuse);
				DrawVec3Control("Specular", component.specular);

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_selectionContext.RemoveComponent<LightComponent>();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 35.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				SpriteRendererComponent& component = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
				ImGui::Text("Texture");
				uint64_t textureId = 0;
				if (component.texture != nullptr)
				{
					textureId = component.texture->GetRendererId();
				}

				ImGui::ImageButton((void*)textureId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* filepath = (const wchar_t*)payload->Data;
						const std::filesystem::path path = filepath;
						component.texture = Texture2D::Create(path.string());
					}
				}

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_selectionContext.RemoveComponent<SpriteRendererComponent>();
			}
		}

		if (entity.HasComponent<ModelComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(ModelComponent).hash_code(), treeNodeFlags, "Model Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 35.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				ModelComponent* component = &entity.GetComponent<ModelComponent>();
				uint64_t modelId = 0;
				if (component->loading)
				{
					ImGui::ProgressBar(*component->progression, ImVec2(0.0f, 0.0f));
					ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
					ImGui::Text("Progress Bar");
				}
				else
				{
					if (component->model != nullptr)
					{
						ImGui::LabelText("Filename", component->model->GetFilePath().filename().string().c_str());
					}

					ImGui::ImageButton((void*)modelId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
						{
							const wchar_t* filepath = (const wchar_t*)payload->Data;
							const std::filesystem::path path = filepath;
							std::thread([=]()
								{
									component->loading = true;
									component->model = Model::Create(path.string(), false, entity, m_context->GetLoadedTextureMap(), component->progression);
									component->loading = false;
									*component->progression = 0.0f;
								}).detach();
						}
					}
				}


				if (component->model != nullptr && component->model->HasAnimations())
				{
					ImGui::Checkbox("Animation", &component->enableAnimation);
					AnimationInfo selectedAnimation = component->model->GetSelectedAnimation();
					std::vector<AnimationInfo> animations = component->model->GetAnimations();
					if (ImGui::BeginCombo("Action", selectedAnimation.displayName.c_str()))
					{
						for (AnimationInfo currentAnimation : animations)
						{
							bool isSelected = selectedAnimation.id == currentAnimation.id;
							if (ImGui::Selectable(currentAnimation.displayName.c_str(), isSelected))
							{
								selectedAnimation = currentAnimation;
								component->model->SetSelectedAnimation(currentAnimation);
							}

							if (isSelected) {
								ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndCombo();
					}

					ImGui::SliderFloat("Time", selectedAnimation.animationTime.get(), 0.0f, selectedAnimation.duration, "%.3f sec.");
				}

				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_selectionContext.RemoveComponent<ModelComponent>();
			}
		}

		if (entity.HasComponent<SkyboxComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(SkyboxComponent).hash_code(), treeNodeFlags, "Skybox");
			ImGui::SameLine(ImGui::GetWindowWidth() - 35.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				static const TextureOrientationType layout[10]
				{
					TextureOrientationType::None,	TextureOrientationType::Top,	TextureOrientationType::None,	TextureOrientationType::None,
					TextureOrientationType::Left,	TextureOrientationType::Front,	TextureOrientationType::Right,	TextureOrientationType::Back,
					TextureOrientationType::None,	TextureOrientationType::Bottom
				};
				ImGui::Text("Textures");
				for (uint32_t i = 0; i < 10; i++)
				{
					ImGui::PushID(i);
					if ((i % 4) != 0)
					{
						ImGui::SameLine();
					}

					TextureOrientationType type = layout[i];
					const char* buttonName = Enum::ToString(type);
					if (buttonName == nullptr)
					{
						ImGui::Dummy(ImVec2(72, 70));
						ImGui::PopID();
						continue;
					}
					else
					{
						SkyboxComponent& component = entity.GetComponent<SkyboxComponent>();
						uint64_t textureId = component.GetTextureId(type, 0);
						if (textureId == 0)
						{
							ImGui::Button(buttonName, ImVec2(72, 70));
						}
						else
						{
							ImGui::ImageButton((void*)textureId, ImVec2(64, 64), ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f));
						}

						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
							{
								const wchar_t* filepath = (const wchar_t*)payload->Data;
								const std::filesystem::path path = filepath;
								component.SetFace(type, path.string());
							}
							ImGui::EndDragDropTarget();
						}
						ImGui::PopID();
					}

				}
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_selectionContext.RemoveComponent<SkyboxComponent>();
			}
		}

		if (entity.HasComponent<TerrainComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
			bool open = ImGui::TreeNodeEx((void*)typeid(TerrainComponent).hash_code(), treeNodeFlags, "Terrian");
			ImGui::SameLine(ImGui::GetWindowWidth() - 35.0f);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}
			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					removeComponent = true;
				}

				ImGui::EndPopup();
			}

			if (open)
			{
				TerrainComponent& component = entity.GetComponent<TerrainComponent>();
				uint64_t textureId = 0;
				if (component.texture != nullptr)
				{
					textureId = component.texture->GetRendererId();
				}

				ImGui::ImageButton((void*)textureId, ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* filepath = (const wchar_t*)payload->Data;
						const std::filesystem::path path = filepath;
						component.texture = Texture2D::Create(path.string(), TextureType::Height, false);
						component.terrian = CreatePtr<Terrian>(component.texture, entity);
					}
				}
				
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				m_selectionContext.RemoveComponent<TerrainComponent>();
			}
		}
	}
}
