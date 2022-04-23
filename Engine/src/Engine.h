#pragma once

// Core
#include "Engine/Core/Application.h"
#include "Engine/Core/Layer.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/Enum/KeyCodes.h"
#include "Engine/Core/Enum/MouseButtonCodes.h"
#include "Engine/ImGui/ImGuiLayer.h"

// Debug
#include "Engine/Debug/FPSCalculator.h"

// Renderer
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Renderer3D.h"

#include "Engine/Renderer/Buffer/Framebuffer.h"
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Camera/OrthographicCamera.h"
#include "Engine/Renderer/Camera/OrthographicCameraController.h"

#include "Engine/Renderer/Model/Model.h"
#include "Engine/Renderer/Shader/Shader.h"
#include "Engine/Renderer/Shader/ShaderLibrary.h"
#include "Engine/Renderer/Skybox/Skybox.h"
#include "Engine/Renderer/Texture/Texture.h"

// Entity
#include "Engine/Scene/Component.h"
#include "Engine/Scene/Entity.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Scene/ScriptableEntity.h"

// Util
#include "Engine/Util/PlatformUtil.h"

// Math
#include "Engine/Math/Math.h"
