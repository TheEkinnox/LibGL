#pragma once
#include <Vector/Vector2.h>

#include "Core/Color.h"

namespace LibGL::Rendering::Resources
{
    class Shader;
    class Texture;

	class Material
	{
	public:
		struct Maps
		{
			const Texture* m_diffuse;
			const Texture* m_specular;
			const Texture* m_normal;
		};

		struct ColorData
		{
			Color	m_tint = Color::white;
			Color	m_specular = Color(.2f, .2f, .2f);
		};

		struct UVModifiers
		{
			LibMath::Vector2	m_offset;
			LibMath::Vector2	m_scale;
		};

		Material() = delete;
		Material(Shader& shader, const Maps& matMaps, const UVModifiers& uvModifiers, const ColorData& colors, float shininess);

		Material(const Material& other) = default;
		Material(Material&& other) = default;
		~Material() = default;

		Material& operator=(const Material& other) = default;
		Material& operator=(Material&& other) = default;

		/**
		 * \brief Gets the material's shader
		 * \return A reference to the material's shader
		 */
		Shader& getShader() const;

		/**
		 * \brief Gets the material's diffuse map
		 * \return The material's diffuse map
		 */
		const Texture& getDiffuseMap() const;

		/**
		 * \brief Gets the material's specular map
		 * \return The material's specular map
		 */
		const Texture& getSpecularMap() const;

		/**
		 * \brief Gets the material's normal map
		 * \return The material's normal map
		 */
		const Texture& getNormalMap() const;

		/**
		 * \brief Gets the material's UV offset
		 * \return The material's UV offset
		 */
		LibMath::Vector2 getUVOffset() const;

		/**
		 * \brief Gets the material's UV scale
		 * \return The material's UV scale
		 */
		LibMath::Vector2 getUVScale() const;

		/**
		 * \brief Gets the material's tint
		 * \return The material's tint
		 */
		Color getTint() const;

		/**
		 * \brief Gets the material's default specular color
		 * \return The material's default specular color
		 */
		Color getSpecularColor() const;

		/**
		 * \brief Sets the material's shader to the given value
		 * \param shader The material's new shader
		 */
		void setShader(Shader& shader);

		/**
		 * \brief Sets the material's diffuse map to the given value
		 * \param diffuseMap The material's new diffuse map
		 */
		void setDiffuseMap(const Texture* diffuseMap);

		/**
		 * \brief Sets the material's diffuse map to the given value
		 * \param specularMap The material's new normal map
		 */
		void setSpecularMap(const Texture* specularMap);

		/**
		 * \brief Sets the material's diffuse map to the given value
		 * \param normalMap The material's new normal map
		 */
		void setNormalMap(const Texture* normalMap);

		/**
		 * \brief Sets the material's UV offset to the given value
		 * \param uvOffset The material's new UV offset
		 */
		void setUVOffset(const LibMath::Vector2& uvOffset);

		/**
		 * \brief Sets the material's UV scale to the given value
		 * \param uvScale The material's new UV scale
		 */
		void setUVScale(const LibMath::Vector2& uvScale);

		/**
		 * \brief Sets the material's tint to the given value
		 * \param tint The material's new tint
		 */
		void setTint(const Color& tint);

		/**
		 * \brief Sets the material's default specular color to the given value
		 * \note This color is used only if there is no specular map
		 * \param specular The material's new specular color
		 */
		void setSpecularColor(const Color& specular);

		/**
		 * \brief Uses the current material
		 */
		void use() const;

	private:
		Maps			m_maps;
		UVModifiers		m_uvModifiers;
		ColorData		m_colors;
		Shader*			m_shader;
		float			m_shininess;
	};
}
