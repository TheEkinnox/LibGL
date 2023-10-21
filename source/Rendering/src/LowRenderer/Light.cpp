#include "LowRenderer/Light.h"
#include "Resources/Shader.h"

using namespace LibMath;

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

	Matrix4 Light::getMatrix() const
	{
		Matrix4 lightMat;

		lightMat[lightMat.getIndex(0, 0)] = m_color.m_r;
		lightMat[lightMat.getIndex(1, 0)] = m_color.m_g;
		lightMat[lightMat.getIndex(2, 0)] = m_color.m_b;
		lightMat[lightMat.getIndex(3, 0)] = m_color.m_a;

		lightMat[lightMat.getIndex(3, 3)] = static_cast<float>(ELightType::AMBIENT);

		return lightMat;
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

	Matrix4 DirectionalLight::getMatrix() const
	{
		Matrix4 lightMat(Light::getMatrix());

		lightMat[lightMat.getIndex(0, 1)] = m_direction.m_x;
		lightMat[lightMat.getIndex(0, 2)] = m_direction.m_y;
		lightMat[lightMat.getIndex(0, 3)] = m_direction.m_z;

		lightMat[lightMat.getIndex(3, 3)] = static_cast<float>(ELightType::DIRECTIONAL);

		return lightMat;
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

	Matrix4 PointLight::getMatrix() const
	{
		Matrix4 lightMat(Light::getMatrix());

		lightMat[lightMat.getIndex(1, 1)] = m_position.m_x;
		lightMat[lightMat.getIndex(1, 2)] = m_position.m_y;
		lightMat[lightMat.getIndex(1, 3)] = m_position.m_z;

		lightMat[lightMat.getIndex(2, 1)] = m_attenuationData.m_constant;
		lightMat[lightMat.getIndex(2, 2)] = m_attenuationData.m_linear;
		lightMat[lightMat.getIndex(2, 3)] = m_attenuationData.m_quadratic;

		lightMat[lightMat.getIndex(3, 3)] = static_cast<float>(ELightType::POINT);

		return lightMat;
	}

	SpotLight::SpotLight(const Light& light, const Vector3& position,
		const Vector3& direction, const AttenuationData& attenuationData,
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

	Matrix4 SpotLight::getMatrix() const
	{
		Matrix4 lightMat(Light::getMatrix());

		lightMat[lightMat.getIndex(0, 1)] = m_direction.m_x;
		lightMat[lightMat.getIndex(0, 2)] = m_direction.m_y;
		lightMat[lightMat.getIndex(0, 3)] = m_direction.m_z;

		lightMat[lightMat.getIndex(1, 1)] = m_position.m_x;
		lightMat[lightMat.getIndex(1, 2)] = m_position.m_y;
		lightMat[lightMat.getIndex(1, 3)] = m_position.m_z;

		lightMat[lightMat.getIndex(2, 1)] = m_attenuationData.m_constant;
		lightMat[lightMat.getIndex(2, 2)] = m_attenuationData.m_linear;
		lightMat[lightMat.getIndex(2, 3)] = m_attenuationData.m_quadratic;

		lightMat[lightMat.getIndex(3, 1)] = m_cutoff.m_inner;
		lightMat[lightMat.getIndex(3, 2)] = m_cutoff.m_outer;

		lightMat[lightMat.getIndex(3, 3)] = static_cast<float>(ELightType::SPOT);

		return lightMat;
	}
}
