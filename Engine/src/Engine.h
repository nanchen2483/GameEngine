#pragma once

// Core
#include "Engine/Core/Application.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Layer/Layer.h"
#include "Engine/Core/Log/Log.h"
#include "Engine/Core/Window/Input.h"

// Debug
#include "Engine/Debug/FPSCalculator.h"
#include "Engine/Renderer/Shadow/Debug/ShadowDebug.h"

// Renderer
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Camera/OrthographicCamera.h"
#include "Engine/Renderer/Camera/OrthographicCameraController.h"
#include "Engine/Renderer/Model/Model.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"
#include "Engine/Renderer/RendererCommand.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Shader/ShaderLibrary.h"
#include "Engine/Renderer/Skybox/Skybox.h"
#include "Engine/Renderer/Terrain/Terrain.h"
#include "Engine/Renderer/Texture/Texture.h"
#include "Engine/Renderer/Vertex/Buffer/Framebuffer.h"

// Scene
#include "Engine/Scene/Component.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/ScriptableEntity.h"

// Math
#include "Engine/Math/Math.h"

// Util
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Util/PlatformUtil.h"
