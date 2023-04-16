#include "LowRenderer/Light.h"
#include "Resources/Shader.h"

namespace LibGL::Rendering
{
	Light::Light(const Color& color) :
		m_color(color)
	{
	}

	void Light::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		shader.setUniformVec4(uniformName + ".color", m_color.rgba());
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

	AttenuationData::AttenuationData(const float range) :
		AttenuationData(1.f, 4.5f / range, 75.f / (range * range))
	{
	}

	AttenuationData::AttenuationData(const float constant, const float linear, const float quadratic) :
		m_constant(constant), m_linear(linear), m_quadratic(quadratic)
	{
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
		const Cutoff& cutoff)
		: Light(light), m_position(position),m_direction(direction),
		m_attenuationData(attenuationData), m_cutoff(cutoff)
	{
	}

	void SpotLight::setupUniform(const std::string& uniformName, const Resources::Shader& shader) const
	{
		Light::setupUniform(uniformName, shader);

		shader.setUniformVec3(uniformName + ".position", m_position);
		shader.setUniformVec3(uniformName + ".direction", m_direction);

		shader.setUniformFloat(uniformName + ".cutoff", m_cutoff.m_inner);
		shader.setUniformFloat(uniformName + ".outerCutoff", m_cutoff.m_outer);

		shader.setUniformFloat(uniformName + ".constant", m_attenuationData.m_constant);
		shader.setUniformFloat(uniformName + ".linear", m_attenuationData.m_linear);
		shader.setUniformFloat(uniformName + ".quadratic", m_attenuationData.m_quadratic);
	}
}
