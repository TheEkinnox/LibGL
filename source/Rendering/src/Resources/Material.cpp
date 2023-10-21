#include "Resources/Material.h"

#include "Enums/EMaterialMap.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

using namespace LibMath;

namespace LibGL::Rendering::Resources
{
	Material::Material(const Shader& shader, const Maps& matMaps,
		const UVModifiers& uvModifiers, const ColorData& colors, const float shininess) :
		m_maps(matMaps), m_uvModifiers(uvModifiers), m_colors(colors),
		m_shader(&shader), m_shininess(shininess)
	{
	}

	const Shader& Material::getShader() const
	{
		return *m_shader;
	}

	const Texture& Material::getDiffuseMap() const
	{
		if (m_maps.m_diffuse == nullptr)
			return Texture::getDefault();

		return *m_maps.m_diffuse;
	}

	const Texture& Material::getSpecularMap() const
	{
		if (m_maps.m_specular == nullptr)
			return Texture::getDefault();

		return *m_maps.m_specular;
	}

	const Texture& Material::getNormalMap() const
	{
		if (m_maps.m_normal == nullptr)
			return Texture::getDefault();

		return *m_maps.m_normal;
	}

	Vector2 Material::getUVOffset() const
	{
		return m_uvModifiers.m_offset;
	}

	Vector2 Material::getUVScale() const
	{
		return m_uvModifiers.m_scale;
	}

	Color Material::getTint() const
	{
		return m_colors.m_tint;
	}

	Color Material::getSpecularColor() const
	{
		return m_colors.m_specular;
	}

	void Material::setShader(const Shader& shader)
	{
		m_shader = &shader;
	}

	void Material::setDiffuseMap(const Texture* diffuseMap)
	{
		m_maps.m_diffuse = diffuseMap;
	}

	void Material::setSpecularMap(const Texture* specularMap)
	{
		m_maps.m_specular = specularMap;
	}

	void Material::setNormalMap(const Texture* normalMap)
	{
		m_maps.m_diffuse = normalMap;
	}

	void Material::setUVOffset(const Vector2& uvOffset)
	{
		m_uvModifiers.m_offset = uvOffset;
	}

	void Material::setUVScale(const Vector2& uvScale)
	{
		m_uvModifiers.m_scale = uvScale;
	}

	void Material::setTint(const Color& tint)
	{
		m_colors.m_tint = tint;
	}

	void Material::setSpecularColor(const Color& specular)
	{
		m_colors.m_specular = specular;
	}

	void Material::use() const
	{
		m_shader->use();

		int usedMaps = 0;

		if (m_maps.m_normal != nullptr)
			usedMaps |= static_cast<int>(EMaterialMap::NORMAL);

		if (m_maps.m_specular != nullptr)
			usedMaps |= static_cast<int>(EMaterialMap::SPECULAR);

		if (m_maps.m_diffuse != nullptr)
			usedMaps |= static_cast<int>(EMaterialMap::DIFFUSE);

		getDiffuseMap().bind(0);
		getSpecularMap().bind(1);
		getNormalMap().bind(2);

		m_shader->setUniformVec4("u_material.tint", getTint().rgba());
		m_shader->setUniformVec4("u_material.specularColor", getSpecularColor().rgba());
		m_shader->setUniformVec2("u_material.uvOffset", getUVOffset());
		m_shader->setUniformVec2("u_material.uvScale", getUVScale());
		m_shader->setUniformFloat("u_material.shininess", m_shininess);
		m_shader->setUniformInt("u_material.usedMaps", usedMaps);
		m_shader->setUniformInt("u_material.diffuse", 0);
		m_shader->setUniformInt("u_material.specular", 1);
		m_shader->setUniformInt("u_material.normal", 2);
	}
}
