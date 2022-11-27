#include "SceneHierarchyPanel.h"
#include "Extension/ImGuiExtension.h"

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
			ImGui::Separator();

			DrawComponents(m_selectionContext);
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

			ImGuiExtension::InputText("Name", tag);
		}

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<TransformComponent>())
		{
			ImGuiExtension::DrawSection("Transform",
				[&]()
				{
					Transform& transform = entity.GetComponent<TransformComponent>();
					ImGuiExtension::DrawVec3Control("Translation", transform.translation);
					glm::vec3 rotation = glm::degrees(transform.rotation);
					ImGuiExtension::DrawVec3Control("Rotation", rotation);
					transform.rotation = glm::radians(rotation);
					ImGuiExtension::DrawVec3Control("Scale", transform.scale, 1.0f);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<TransformComponent>();
				});
		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGuiExtension::DrawSection("Camera",
				[&]()
				{
					CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
					SceneCamera& camera = cameraComponent.camera;

					ImGui::Checkbox("Primary", &cameraComponent.primary);
					std::map<CameraProjectionType, std::string> projectionTypeMap =
					{
						{ CameraProjectionType::Perspective, "Perspective" },
						{ CameraProjectionType::Orthographic, "Orthographic" }
					};
					std::string selectedProjectionString = projectionTypeMap[camera.GetProjectionType()];
					if (ImGui::BeginCombo("Projection", selectedProjectionString.c_str()))
					{
						for (auto& it : projectionTypeMap)
						{
							CameraProjectionType currentProjectionType = it.first;
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
				
					if (camera.GetProjectionType() == CameraProjectionType::Perspective)
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

					if (camera.GetProjectionType() == CameraProjectionType::Orthographic)
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
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<CameraComponent>();
				});
		}

		if (entity.HasComponent<LightComponent>())
		{
			ImGuiExtension::DrawSection("Light",
				[&]()
				{
					LightComponent& component = entity.GetComponent<LightComponent>();
					ImGuiExtension::DrawFloatControl("Constant", component.constant, 1.0f, 0.01f, 0.0f, 1.0f);
					ImGuiExtension::DrawFloatControl("Linear", component.linear, 0.9f, 0.001f, 0.001f, 1.0f);
					ImGuiExtension::DrawFloatControl("Quadratic", component.quadratic, 0.03f, 0.001f, 0.01f, 2.0f);
					ImGui::NewLine();
					ImGuiExtension::DrawVec3Control("Ambient", component.ambient);
					ImGuiExtension::DrawVec3Control("Diffuse", component.diffuse);
					ImGuiExtension::DrawVec3Control("Specular", component.specular);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<LightComponent>();
				});
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGuiExtension::DrawSection("Sprite Renderer",
				[&]()
				{
					SpriteRendererComponent& component = entity.GetComponent<SpriteRendererComponent>();
					ImGuiExtension::DrawColorEdit4Control("Color", component.color);
					ImGuiExtension::DrawTextureControl("Texture", component.texture);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<SpriteRendererComponent>();
				});
		}

		if (entity.HasComponent<ModelComponent>())
		{
			ImGuiExtension::DrawSection("Model",
				[&]()
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
										component->model = Model::Create(path.string(), false, entity, component->progression);
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
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<ModelComponent>();
				});
		}

		if (entity.HasComponent<SkyboxComponent>())
		{
			ImGuiExtension::DrawSection("Skybox",
				[&]()
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
							Ptr<Image> image = component.images[(uint32_t)type];
							if (image == nullptr)
							{
								ImGui::Button(buttonName, ImVec2(72, 70));
							}
							else
							{
								Ptr<Texture> texture = TextureLibrary::GetInstance()->Load(image, TextureType::Skybox);
								ImGui::ImageButton((void*)texture->GetRendererId(), ImVec2(64, 64), ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f));
							}

							if (ImGui::BeginDragDropTarget())
							{
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
								{
									const wchar_t* filepath = (const wchar_t*)payload->Data;
									const std::filesystem::path path = filepath;
								
									image = CreatePtr<Image>(path.string(), true);
									component.images[(uint32_t)type] = image;
									if (std::find(component.images.begin(), component.images.end(), nullptr) == component.images.end())
									{
										if (component.skybox == nullptr)
										{
											component.skybox = CreatePtr<Skybox>(component.images);
										}
										else
										{
											component.skybox->GetTexture()->SetData(image->GetData(), type, image->GetSize());
										}
									}
								}
								ImGui::EndDragDropTarget();
							}
							ImGui::PopID();
						}

					}
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<SkyboxComponent>();
				});
		}

		if (entity.HasComponent<TerrainComponent>())
		{
			ImGuiExtension::DrawSection("Terrian",
				[&]()
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
							component.texture = Texture2D::Create(path.string(), TextureType::Height);
							component.terrain = Terrain::Create(TerrainType::Default, component.texture, entity);
						}
					}
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<TerrainComponent>();
				});
		}
	}
}
