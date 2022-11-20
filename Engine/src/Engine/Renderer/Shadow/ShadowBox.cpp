#include "enginepch.h"
#include "ShadowBox.h"

#include "Engine/Renderer/RendererCommand.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Engine
{
	ShadowBox::ShadowBox()
	{
		m_shader = Shader::Create("assets/shaders/Shadow.glsl");
		
		FramebufferSpecification fbSpec;
		fbSpec.attachments = FramebufferAttachmentSpecification({ FramebufferTextureFormat::DEPTH_ARRAY });
		fbSpec.width = m_depthMapResolution;
		fbSpec.height = m_depthMapResolution;
		fbSpec.arraySize = m_shadowInfo.levels.size();
		m_framebuffer = Framebuffer::Create(fbSpec);

		m_lightSpaceMatrixUniformBuffer = UniformBuffer::Create(3, {
				BufferLayoutType::Std140,
				{
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Mat4 },
					{ ShaderDataType::Mat4 },
				}
			});
	}

	void ShadowBox::Bind()
	{
		m_shader->Bind();
		m_framebuffer->Bind();
		RendererCommand::Clear();
		RendererCommand::CullFace(FaceCulling::BACK);
	}

	void ShadowBox::Ubind()
	{
		m_framebuffer->Unbind();
		m_shader->Unbind();
	}

	void ShadowBox::BindTexture()
	{
		m_framebuffer->BindDepthTexture(m_shadowInfo.depthTextureSlot);
	}

	void ShadowBox::Update(glm::mat4 viewMatrix, float fov, float aspectRatio)
	{
		m_cameraViewMatrix = viewMatrix;
		m_cameraFOV = fov;
		m_aspectRatio = aspectRatio;

		std::vector<glm::mat4> matrices = GetLightSpaceMatrices();
		std::vector<const void*> matricesData(matrices.size());
		for (uint32_t i = 0; i < matrices.size(); i++)
		{
			matricesData[i] = &matrices[i];
		}

		m_lightSpaceMatrixUniformBuffer->SetData(matricesData);
	}

	std::vector<glm::mat4> ShadowBox::GetLightSpaceMatrices()
	{
		std::vector<glm::mat4> lightSpaceMatrices;
		for (size_t i = 0; i < m_shadowInfo.levels.size(); i++)
		{
			if (i == 0)
			{
				lightSpaceMatrices.push_back(GetLightSpaceMatrix(m_shadowInfo.nearPlane, m_shadowInfo.levels[i]));
			}
			else
			{
				lightSpaceMatrices.push_back(GetLightSpaceMatrix(m_shadowInfo.levels[i - 1], m_shadowInfo.levels[i]));
			}
		}
		return lightSpaceMatrices;
	}

	glm::mat4 ShadowBox::GetLightSpaceMatrix(const float nearPlane, const float farPlane)
	{
		const glm::mat4 projection = glm::perspective(glm::radians(m_cameraFOV), m_aspectRatio, nearPlane, farPlane);
		const std::vector<glm::vec4> frustumCorners = GetFrustumCornersWorldSpace(projection * m_cameraViewMatrix);

		glm::vec3 center = glm::vec3(0);
		for (const glm::vec4& corner : frustumCorners)
		{
			center += glm::vec3(corner);
		}
		center /= frustumCorners.size();

		const glm::mat4 lightView = glm::lookAt(center + m_lightDirection, center, m_up);

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::min();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::min();
		for (const glm::vec4& v : frustumCorners)
		{
			const glm::vec4 trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		// Tune this parameter according to the scene
		constexpr float zMult = 10.0f;
		if (minZ < 0)
		{
			minZ *= zMult;
		}
		else
		{
			minZ /= zMult;
		}
		if (maxZ < 0)
		{
			maxZ /= zMult;
		}
		else
		{
			maxZ *= zMult;
		}

		const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

		return lightProjection * lightView;
	}
	
	std::vector<glm::vec4> ShadowBox::GetFrustumCornersWorldSpace(const glm::mat4& projview)
	{
		const glm::mat4 inv = glm::inverse(projview);

		std::vector<glm::vec4> frustumCorners;
		for (uint32_t x = 0; x < 2; x++)
		{
			for (uint32_t y = 0; y < 2; y++)
			{
				for (uint32_t z = 0; z < 2; z++)
				{
					const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
					frustumCorners.push_back(pt / pt.w);
				}
			}
		}

		return frustumCorners;
	}
}
