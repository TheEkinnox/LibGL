#pragma once
#include "Maths/Vector/Vector4.h"

namespace My
{
	namespace Resources
	{
		class Shader;
	}
}

namespace My::Rendering
{
	using namespace LibMath;
	using namespace Resources;

	struct Light
	{
		Vector4	m_ambient;
		Vector4	m_diffuse;
		Vector4	m_specular;

		Light() = default;
		Light(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular);
		Light(const Light&) = default;
		Light(Light&&) = default;
		virtual ~Light() = default;

		Light& operator=(const Light&) = default;
		Light& operator=(Light&&) = default;

		/**
		 * \brief Sets up the uniform variables for the current light
		 * in the given shader
		 * \param uniformName The name of the light's uniform variable
		 / in the given shader
		 * \param shader The shader for which the uniform should be set
		 */
		virtual void setupUniform(const std::string& uniformName, const Shader& shader) const;
	};

	struct DirectionalLight : Light
	{
		Vector3	m_direction;

		DirectionalLight() = default;
		DirectionalLight(const Light& light, const Vector3& direction);
		DirectionalLight(const DirectionalLight&) = default;
		DirectionalLight(DirectionalLight&&) = default;
		~DirectionalLight() override = default;

		DirectionalLight& operator=(const DirectionalLight&) = default;
		DirectionalLight& operator=(DirectionalLight&&) = default;

		/**
		 * \brief Sets up the uniform variables for the current light
		 * in the given shader
		 * \param uniformName The name of the light's uniform variable
		 / in the given shader
		 * \param shader The shader for which the uniform should be set
		 */
		void setupUniform(const std::string& uniformName, const Shader& shader) const override;
	};

	struct AttenuationData
	{
		float	m_constant = 1;
		float	m_linear = 0;
		float	m_quadratic = 0;
	};

	struct PointLight : Light
	{
		Vector3			m_position;
		AttenuationData	m_attenuationData;

		PointLight() = default;
		PointLight(const Light& light, const Vector3& position,
			const AttenuationData& attenuationData);

		PointLight(const PointLight&) = default;
		PointLight(PointLight&&) = default;
		~PointLight() override = default;

		PointLight& operator=(const PointLight&) = default;
		PointLight& operator=(PointLight&&) = default;

		/**
		 * \brief Sets up the uniform variables for the current light
		 * in the given shader
		 * \param uniformName The name of the light's uniform variable
		 / in the given shader
		 * \param shader The shader for which the uniform should be set
		 */
		void setupUniform(const std::string& uniformName, const Shader& shader) const override;
	};

	struct SpotLight : Light
	{
		Vector3			m_position;
		Vector3			m_direction;

		AttenuationData	m_attenuationData;

		float			m_cutOff = 0;
		float			m_outerCutoff = 0;

		SpotLight() = default;
		SpotLight(const Light& light, const Vector3& position,
			const Vector3& direction, const AttenuationData& attenuationData,
			float cutOff, float outerCutOff);

		SpotLight(const SpotLight&) = default;
		SpotLight(SpotLight&&) = default;
		~SpotLight() override = default;

		SpotLight& operator=(const SpotLight&) = default;
		SpotLight& operator=(SpotLight&&) = default;

		/**
		 * \brief Sets up the uniform variables for the current light
		 * in the given shader
		 * \param uniformName The name of the light's uniform variable
		 / in the given shader
		 * \param shader The shader for which the uniform should be set
		 */
		void setupUniform(const std::string& uniformName, const Shader& shader) const override;
	};
}
