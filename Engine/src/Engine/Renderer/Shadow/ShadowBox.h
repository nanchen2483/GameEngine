#pragma once
#include "Engine/Renderer/Buffer/IBuffer.h"
#include "Engine/Renderer/Buffer/IFramebuffer.h"
#include "Engine/Renderer/Shader/IShader.h"

namespace Engine
{
	static const glm::vec3 LIGHT_DIRECTION = glm::normalize(glm::vec3(0.2f, 1.0f, 0.3f));

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
		Ptr<IShader> GetShader() const { return m_shader; }
	private:
		std::vector<glm::mat4> GetLightSpaceMatrices();
		glm::mat4 GetLightSpaceMatrix(const float nearPlane, const float farPlane);
		std::vector<glm::vec4> GetFrustumCornersWorldSpace(const glm::mat4& projview);

		Ptr<IShader> m_shader;
		Ptr<IFramebuffer> m_framebuffer;
		Ptr<IUniformBuffer> m_lightSpaceMatrixUniformBuffer;
		
		// Camera properties
		glm::mat4 m_cameraViewMatrix;
		float m_cameraFOV;
		float m_aspectRatio;

		// Light properties
		const uint32_t m_depthMapResolution = 4096;
		const ShadowBoxInfo m_shadowInfo;
		const glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };
	};
}
