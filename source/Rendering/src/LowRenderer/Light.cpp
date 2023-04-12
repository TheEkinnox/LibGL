#include "LowRenderer/Light.h"
#include "Resources/Shader.h"

namespace LibGL::Rendering
{
	Light::Light(const Color& ambient, const Color& diffuse, const Color& specular)
		: m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
	{
	}

	void Light::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		shader.setUniformVec4(uniformName + ".ambient", m_ambient.rgba());
		shader.setUniformVec4(uniformName + ".diffuse", m_diffuse.rgba());
		shader.setUniformVec4(uniformName + ".specular", m_specular.rgba());
	}

	DirectionalLight::DirectionalLight(const Light& light, const LibMath::Vector3& direction)
		: Light(light), m_direction(direction)
	{
	}

	void DirectionalLight::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		Light::setupUniform(uniformName, shader);

		shader.setUniformVec3(uniformName + ".direction", m_direction);
	}

	PointLight::PointLight(const Light& light, const LibMath::Vector3& position,
		const AttenuationData& attenuationData)
		: Light(light), m_position(position), m_attenuationData(attenuationData)
	{
	}

	void PointLight::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		Light::setupUniform(uniformName, shader);

		shader.setUniformVec3(uniformName + ".position", m_position);

		shader.setUniformFloat(uniformName + ".constant", m_attenuationData.m_constant);
		shader.setUniformFloat(uniformName + ".linear", m_attenuationData.m_linear);
		shader.setUniformFloat(uniformName + ".quadratic", m_attenuationData.m_quadratic);
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

		shader.setUniformVec3(uniformName + ".position", m_position);
		shader.setUniformVec3(uniformName + ".direction", m_direction);

		shader.setUniformFloat(uniformName + ".cutOff", m_cutOff);
		shader.setUniformFloat(uniformName + ".outerCutOff", m_outerCutoff);

		shader.setUniformFloat(uniformName + ".constant", m_attenuationData.m_constant);
		shader.setUniformFloat(uniformName + ".linear", m_attenuationData.m_linear);
		shader.setUniformFloat(uniformName + ".quadratic", m_attenuationData.m_quadratic);
	}
}
