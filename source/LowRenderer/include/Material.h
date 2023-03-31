#pragma once
namespace My
{
	namespace Resources
	{
		class Shader;
		class Texture;
	}
}

namespace My::Rendering
{
	using namespace Resources;

	class Material
	{
	public:
		Material() = delete;
		Material(const Shader& shader, const Texture* diffuse, float shininess);
		Material(const Material& other) = default;
		Material(Material&& other) = default;
		~Material() = default;

		Material& operator=(const Material& other) = default;
		Material& operator=(Material&& other) = default;

		/**
		 * \brief Gets the material's shader
		 * \return A reference to the material's shader
		 */
		const Shader& getShader() const;

		/**
		 * \brief Sets the material's shader to the given value
		 * \param shader The material's new shader
		 */
		void setShader(const Shader& shader);

		/**
		 * \brief Gets the material's diffuse texture
		 * \return The material's diffuse texture
		 */
		const Texture* getDiffuse() const;

		/**
		 * \brief Sets the material's diffuse texture to the given value
		 * \param texture The material's new diffuse texture
		 */
		void setDiffuse(const Texture* texture);

		void use() const;

	private:
		const Shader*	m_shader;
		const Texture*	m_diffuse;
		float			m_shininess;
	};
}
