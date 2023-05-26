#pragma once

// Core
#include "Engine/Core/Application.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Layer/Layer.h"
#include "Engine/Core/Log/Log.h"
#include "Engine/Core/System/System.h"
#include "Engine/Core/Window/Input.h"
#include "Engine/Configuration/Configuration.h"

// Debug
#include "Engine/Debug/FPSCalculator.h"
#include "Engine/Renderer/Shadow/Debug/ShadowDebug.h"

// Effect
#include "Engine/Effect/BloomEffect.h"

// File
#include "Engine/File/File.h"
#include "Engine/File/FileDialog.h"
#include "Engine/File/FileExtension.h"
#include "Engine/File/FileType.h"

// Renderer
#include "Engine/Factory/BufferFactory.h"
#include "Engine/Factory/VertexArrayFactory.h"
#include "Engine/Factory/FramebufferFactory.h"
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/RendererCommand.h"

// Scene
#include "Engine/Scene/Entity/Entity.h"
#include "Engine/Scene/Scene.h"

// Component
#include "Engine/Scene/Component/AnimationComponent.h"
#include "Engine/Scene/Component/CameraComponent.h"
#include "Engine/Scene/Component/PhysicsComponent.h"
#include "Engine/Scene/Component/LightComponent.h"
#include "Engine/Scene/Component/NativeScriptComponent.h"
#include "Engine/Scene/Component/MeshComponent.h"
#include "Engine/Scene/Component/SkyboxComponent.h"
#include "Engine/Scene/Component/SpriteRendererComponent.h"
#include "Engine/Scene/Component/TagComponent.h"
#include "Engine/Scene/Component/TerrainComponent.h"
#include "Engine/Scene/Component/TransformComponent.h"

// Library
#include "Engine/Library/ModelLibrary.h"
#include "Engine/Library/ShaderLibrary.h"
#include "Engine/Library/TextureLibrary.h"

// Math
#include "Engine/Math/Math.h"

// Util
#include "Engine/ImGui/ImGuiLayer.h"
