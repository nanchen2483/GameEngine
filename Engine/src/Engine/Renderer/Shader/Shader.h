#pragma once
#include "Engine/Core/System/Object/IUniqueObject.h"

#include <glm/glm.hpp>
#include <string>

namespace Engine
{
	class Shader : public IUniqueObject
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetBool(const std::string& name, const bool value) = 0;
		virtual void SetInt(const std::string& name, const int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetFloat(const std::string& name, const float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual void SetBlockBinding(const std::string& name, const int value) = 0;

		virtual const std::string& GetName() const = 0;
	private:
		static Ptr<Shader> Create(const std::string& filePath);
		static Ptr<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

		friend class ShaderLibrary;
	};
}
