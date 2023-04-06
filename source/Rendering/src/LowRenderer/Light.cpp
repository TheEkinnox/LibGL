#include "LowRenderer/Light.h"
#include "Resources/Shader.h"

#include <glad/glad.h>

namespace LibGL::Rendering
{
	Light::Light(const LibMath::Vector4& ambient, const LibMath::Vector4& diffuse, const LibMath::Vector4& specular)
		: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
	{
	}

	void Light::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		GLint uniformLoc = shader.getUniformLocation(uniformName + ".ambient");
		glUniform4fv(uniformLoc, 1, m_ambient.getArray());

		uniformLoc = shader.getUniformLocation(uniformName + ".diffuse");
		glUniform4fv(uniformLoc, 1, m_diffuse.getArray());

		uniformLoc = shader.getUniformLocation(uniformName + ".specular");
		glUniform4fv(uniformLoc, 1, m_specular.getArray());
	}

	DirectionalLight::DirectionalLight(const Light& light, const LibMath::Vector3& direction)
		: Light(light), m_direction(direction)
	{
	}

	void DirectionalLight::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		Light::setupUniform(uniformName, shader);

		const GLint uniformLoc = shader.getUniformLocation(uniformName + ".direction");
		glUniform3fv(uniformLoc, 1, m_direction.getArray());
	}

	PointLight::PointLight(const Light& light, const LibMath::Vector3& position,
		const AttenuationData& attenuationData)
		: Light(light), m_position(position), m_attenuationData(attenuationData)
	{
	}

	void PointLight::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		Light::setupUniform(uniformName, shader);

		GLint uniformLoc = shader.getUniformLocation(uniformName + ".position");
		glUniform3fv(uniformLoc, 1, m_position.getArray());

		uniformLoc = shader.getUniformLocation(uniformName + ".constant");
		glUniform1f(uniformLoc, m_attenuationData.m_constant);

		uniformLoc = shader.getUniformLocation(uniformName + ".linear");
		glUniform1f(uniformLoc, m_attenuationData.m_linear);

		uniformLoc = shader.getUniformLocation(uniformName + ".quadratic");
		glUniform1f(uniformLoc, m_attenuationData.m_quadratic);
	}

	SpotLight::SpotLight(const Light& light, const LibMath::Vector3& position,
		const LibMath::Vector3& direction, const AttenuationData& attenuationData,
		const float cutOff, const float outerCutOff)
		: Light(light), m_position(position),m_direction(direction),
		m_attenuationData(attenuationData), m_cutOff(cutOff), m_outerCutoff(outerCutOff)
	{
	}

	void SpotLight::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		Light::setupUniform(uniformName, shader);

		GLint uniformLoc = shader.getUniformLocation(uniformName + ".position");
		glUniform3fv(uniformLoc, 1, m_position.getArray());

		uniformLoc = shader.getUniformLocation(uniformName + ".direction");
		glUniform3fv(uniformLoc, 1, m_direction.getArray());

		uniformLoc = shader.getUniformLocation(uniformName + ".cutOff");
		glUniform1f(uniformLoc, m_cutOff);

		uniformLoc = shader.getUniformLocation(uniformName + ".outerCutOff");
		glUniform1f(uniformLoc, m_outerCutoff);

		uniformLoc = shader.getUniformLocation(uniformName + ".constant");
		glUniform1f(uniformLoc, m_attenuationData.m_constant);

		uniformLoc = shader.getUniformLocation(uniformName + ".linear");
		glUniform1f(uniformLoc, m_attenuationData.m_linear);

		uniformLoc = shader.getUniformLocation(uniformName + ".quadratic");
		glUniform1f(uniformLoc, m_attenuationData.m_quadratic);
	}
}
