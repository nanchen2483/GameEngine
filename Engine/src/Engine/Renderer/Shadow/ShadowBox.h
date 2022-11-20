#pragma once
#include "Engine/Renderer/Buffer/Buffer.h"
#include "Engine/Renderer/Buffer/Framebuffer.h"
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Shader/Shader.h"

namespace Engine
{
	struct ShadowBoxInfo
	{
		const uint32_t depthTextureSlot = 40;
		const float nearPlane = 0.1f;
		const float farPlane = 1000.0f;
		const std::vector<float> levels{ farPlane / 50.0f, farPlane / 25.0f, farPlane / 10.0f, farPlane / 2.0f, farPlane };
	};

	class ShadowBox
	{
	public:
		ShadowBox();
		void Bind();
		void Ubind();
		void BindTexture();

		void Update(glm::mat4 viewMatrix, float fov, float aspectRatio);
		Ptr<Shader> GetShader() const { return m_shader; }
	private:
		std::vector<glm::mat4> GetLightSpaceMatrices();
		glm::mat4 GetLightSpaceMatrix(const float nearPlane, const float farPlane);
		std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& projview);

		Ptr<Shader> m_shader;
		Ptr<Framebuffer> m_framebuffer;
		Ptr<UniformBuffer> m_lightSpaceMatrixUniformBuffer;
		
		// Camera properties
		glm::mat4 m_cameraViewMatrix;
		float m_cameraFOV;
		float m_aspectRatio;

		// Light properties
		const glm::vec3 m_lightDirection = glm::normalize(glm::vec3(0.2f, 1.0f, 0.3f));
		const uint32_t m_depthMapResolution = 4096;
		const ShadowBoxInfo m_shadowInfo;
		const glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };
	};
}
