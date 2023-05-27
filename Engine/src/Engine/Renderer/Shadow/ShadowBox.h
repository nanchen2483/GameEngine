#pragma once
#include "Engine/Renderer/Buffer/IUniformBuffer.h"
#include "Engine/Renderer/Framebuffer/IFramebuffer.h"
#include "Engine/Renderer/Shader/IShader.h"

namespace Engine
{
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
		const glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };
	};
}
