#include "HierarchyPanel.h"
#include "Extension/ImGuiExtension.h"

#include <filesystem>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <thread>
#include <IconFontCppHeaders/IconsFontAwesome6.h>

namespace Engine
{
	HierarchyPanel::HierarchyPanel(const Ptr<Scene>& context)
	{
		SetContext(context);
	}

	void HierarchyPanel::SetContext(const Ptr<Scene>& context)
	{
		m_context = context;
		m_selectionContext = {};
	}

	void HierarchyPanel::OnImGuiRender()
	{
		if (m_openOutliner)
		{
			if (ImGui::Begin(ICON_FA_SITEMAP " Outliner", &m_openOutliner))
			{
				m_context->m_registry.each([&](entt::entity entityId) {
					Entity entity = Entity{ entityId, m_context.get() };
					DrawEntityNode(entity);
				});

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				{
					m_selectionContext = {};
				}

				// Right-click on blank space
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Create Empty Entity"))
					{
						m_context->CreateEntity("Empty Entity");
					}

					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}

		if (m_openDetails)
		{
			if (ImGui::Begin(ICON_FA_PENCIL " Details", &m_openDetails))
			{
				if (m_selectionContext)
				{
					DrawComponents(m_selectionContext);
				}
			}
			ImGui::End();
		}
	}
	
	void HierarchyPanel::SetSelectedEntity(Entity& entity)
	{
		m_selectionContext = entity;
	}

	void HierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().tag;
		bool isSelected = m_selectionContext == entity;;
		ImGuiExtension::DrawEntitySection(tag, isSelected,
			[&]()
			{
				m_selectionContext = entity;
			},
			[&]() {},
			[&]()
			{
				m_context->DestoryEntity(entity);
				if (isSelected)
				{
					m_selectionContext = {};
				}
			});
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

	void HierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			std::string& tag = entity.GetComponent<TagComponent>().tag;
			ImGuiExtension::DrawPropertyTagSection(tag, "+ Add", { 100.0f, 25.f },
				[&]()
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

					if (!m_selectionContext.HasComponent<MeshComponent>() && ImGui::MenuItem("Mesh"))
					{
						m_selectionContext.AddComponent<MeshComponent>();
						ImGui::CloseCurrentPopup();
					}
					
					if (m_selectionContext.HasComponent<MeshComponent>())
					{
						if (!m_selectionContext.HasComponent<AnimationComponent>() && ImGui::MenuItem("Animation"))
						{
							m_selectionContext.AddComponent<AnimationComponent>();
							ImGui::CloseCurrentPopup();
						}

						if (!m_selectionContext.HasComponent<CollisionComponent>() && ImGui::MenuItem("Collision"))
						{
							m_selectionContext.AddComponent<CollisionComponent>();
							ImGui::CloseCurrentPopup();
						}
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
				});
		}

		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<TransformComponent>())
		{
			ImGuiExtension::DrawPropertySection("Transform",
				[&]()
				{
					Transform& transform = entity.GetComponent<TransformComponent>();
					ImGuiExtension::DrawVec3SubSection("Location", transform.translation);
					glm::vec3 rotation = glm::degrees(transform.rotation);
					ImGuiExtension::DrawVec3SubSection("Rotation", rotation);
					transform.rotation = glm::radians(rotation);
					ImGuiExtension::DrawVec3SubSection("Scale", transform.scale, 1.0f);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<TransformComponent>();
				});
		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGuiExtension::DrawPropertySection("Camera",
				[&]()
				{
					CameraComponent& cameraComponent = entity.GetComponent<CameraComponent>();
					SceneCamera& camera = cameraComponent.camera;

					ImGuiExtension::DrawCheckboxSubSection("Primary", &cameraComponent.primary);
					std::unordered_map<uint32_t, std::string> projectionTypeMap =
					{
						{ (uint32_t)CameraProjectionType::Perspective, "Perspective" },
						{ (uint32_t)CameraProjectionType::Orthographic, "Orthographic" }
					};
					ImGuiExtension::DrawBeginComboSubSection("Projection", projectionTypeMap[(uint32_t)camera.GetProjectionType()], projectionTypeMap,
						[&](uint32_t selected)
						{
							camera.SetProjectionType((CameraProjectionType)selected);
						});

					if (camera.GetProjectionType() == CameraProjectionType::Perspective)
					{
						float FOV = camera.GetPerspectiveFOV();
						if (ImGuiExtension::DrawFloatSubSection("FOV", FOV))
						{
							camera.SetPerspectiveFOV(FOV);
						}

						float perspectNear = camera.GetPerspectiveNearClip();
						if (ImGuiExtension::DrawFloatSubSection("Near", perspectNear))
						{
							camera.SetPerspectiveNearClip(perspectNear);
						}

						float perspectFar = camera.GetPerspectiveFarClip();
						if (ImGuiExtension::DrawFloatSubSection("Far", perspectFar))
						{
							camera.SetPerspectiveFarClip(perspectFar);
						}
					}

					if (camera.GetProjectionType() == CameraProjectionType::Orthographic)
					{
						float orthoSize = camera.GetOrthographicSize();
						if (ImGuiExtension::DrawFloatSubSection("Size", orthoSize))
						{
							camera.SetOrthographicSize(orthoSize);
						}

						float orthoNear = camera.GetOrthographicNearClip();
						if (ImGuiExtension::DrawFloatSubSection("Near", orthoNear))
						{
							camera.SetOrthographicNearClip(orthoNear);
						}

						float orthoFar = camera.GetOrthographicFarClip();
						if (ImGuiExtension::DrawFloatSubSection("Far", orthoFar))
						{
							camera.SetOrthographicFarClip(orthoFar);
						}

						ImGuiExtension::DrawCheckboxSubSection("Fixed aspect ratio", &cameraComponent.fixedAspectRatio);
					}
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<CameraComponent>();
				});
		}

		if (entity.HasComponent<LightComponent>())
		{
			ImGuiExtension::DrawPropertySection("Light",
				[&]()
				{
					LightComponent& component = entity.GetComponent<LightComponent>();
					ImGuiExtension::DrawFloatSubSection("Constant", component.constant, 1.0f, 0.01f, 0.0f, 1.0f);
					ImGuiExtension::DrawFloatSubSection("Linear", component.linear, 0.9f, 0.001f, 0.001f, 1.0f);
					ImGuiExtension::DrawFloatSubSection("Quadratic", component.quadratic, 0.03f, 0.001f, 0.01f, 2.0f);
					ImGui::NewLine();
					ImGuiExtension::DrawVec3SubSection("Ambient", component.ambient);
					ImGuiExtension::DrawVec3SubSection("Diffuse", component.diffuse);
					ImGuiExtension::DrawVec3SubSection("Specular", component.specular);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<LightComponent>();
				});
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			ImGuiExtension::DrawPropertySection("Sprite Renderer",
				[&]()
				{
					SpriteRendererComponent& component = entity.GetComponent<SpriteRendererComponent>();
					ImGuiExtension::DrawColorEdit4SubSection("Color", component.color);
					ImGuiExtension::DrawTextureSubSection("Texture", component.texture);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<SpriteRendererComponent>();
				});
		}

		if (entity.HasComponent<MeshComponent>())
		{
			ImGuiExtension::DrawPropertySection("Mesh",
				[&]()
				{
					MeshComponent* component = &entity.GetComponent<MeshComponent>();
					if (component->isLoading)
					{
						ImGui::Text("Loading %c", "|/-\\"[(int)(ImGui::GetTime() / 0.05f) & 3]);
					}
					else
					{
						ImGuiExtension::DrawMeshSubSection("Mesh", component->filePath,
							[&](const std::string& filePath)
							{
								std::thread([=]()
									{
										component->isLoading = true;
										component->filePath = filePath;
										Ptr<Model> model = ModelLibrary::GetInstance()->Load(filePath);
										component->meshes = model->GetMeshes();
										component->isLoading = false;
									}).detach();
							});
					}
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<MeshComponent>();
				});
		}

		if (entity.HasComponent<AnimationComponent>())
		{
			ImGuiExtension::DrawPropertySection("Animation",
				[&]()
				{
					AnimationComponent* component = &entity.GetComponent<AnimationComponent>();
					if (component->animations.empty())
					{
						component->animations = ModelLibrary::GetInstance()->Load(entity.GetComponent<MeshComponent>().filePath)->GetAnimations();
					}

					ImGuiExtension::DrawAnimationSubSection(component->animations, component->selectedAnimationIndex, component->isEnabled,
						[&](uint32_t selectedIndex)
						{
							component->selectedAnimationIndex = selectedIndex;
						});
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<AnimationComponent>();
				});
		}

		if (entity.HasComponent<CollisionComponent>())
		{
			ImGuiExtension::DrawPropertySection("Collision",
				[&]()
				{
					CollisionComponent* component = &entity.GetComponent<CollisionComponent>();
					if (component->boundingBox == nullptr)
					{
						component->boundingBox = ModelLibrary::GetInstance()->Load(entity.GetComponent<MeshComponent>().filePath)->GenerateBoundingBox();
					}

					BoundingValue value = component->boundingBox->GetBoundingValue();
					ImGuiExtension::DrawVec3SubSection("Center", value.center);
					ImGuiExtension::DrawVec3SubSection("Extents", value.extents);
					component->boundingBox->SetBoundingValue(value);
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<CollisionComponent>();
				});
		}

		if (entity.HasComponent<SkyboxComponent>())
		{
			ImGuiExtension::DrawPropertySection("Skybox",
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
								ImGui::ImageButton((ImTextureID)(uint64_t)texture->GetRendererId(), ImVec2(64, 64), ImVec2(1.0f, 0.0f), ImVec2(0.0f, 1.0f));
							}

							if (ImGui::BeginDragDropTarget())
							{
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(FileType::Texture.c_str()))
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
			ImGuiExtension::DrawPropertySection("Terrian",
				[&]()
				{
					TerrainComponent& component = entity.GetComponent<TerrainComponent>();
					ImGuiExtension::DrawTextureSubSection("Texture", component.texture, TextureType::Height,
						[&]()
						{
							component.terrain = Terrain::Create(TerrainType::Quadtree, component.texture, entity);
						});
				},
				[&]()
				{
					m_selectionContext.RemoveComponent<TerrainComponent>();
				});
		}
	}
}
