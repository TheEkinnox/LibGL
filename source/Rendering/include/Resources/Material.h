#pragma once

namespace LibGL
{
	namespace Resources
	{
		class Shader;
		class Texture;
	}
}

namespace LibGL::Rendering
{
	class Material
	{
	public:
		Material() = delete;
		Material(const Resources::Shader& shader, const Resources::Texture* diffuse,
			const Resources::Texture* specular, float shininess);

		Material(const Material& other) = default;
		Material(Material&& other) = default;
		~Material() = default;

		Material& operator=(const Material& other) = default;
		Material& operator=(Material&& other) = default;

		/**
		 * \brief Gets the material's shader
		 * \return A reference to the material's shader
		 */
		const Resources::Shader& getShader() const;

		/**
		 * \brief Sets the material's shader to the given value
		 * \param shader The material's new shader
		 */
		void setShader(const Resources::Shader& shader);

		/**
		 * \brief Gets the material's diffuse texture
		 * \return The material's diffuse texture
		 */
		const Resources::Texture* getDiffuse() const;

		/**
		 * \brief Sets the material's diffuse texture to the given value
		 * \param texture The material's new diffuse texture
		 */
		void setDiffuse(const Resources::Texture* texture);

		void use() const;

	private:
		const Resources::Shader*	m_shader;
		const Resources::Texture*	m_diffuse;
		const Resources::Texture*	m_specular;
		float						m_shininess;
	};
}
