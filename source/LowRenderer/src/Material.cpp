#include "Material.h"

#include "Shader.h"
#include "Texture.h"

namespace My::Rendering
{
	Material::Material(const Shader& shader, const Texture* diffuse,
		const float shininess) : m_shader(&shader), m_diffuse(diffuse),
		m_shininess(shininess)
	{
	}

	const Shader& Material::getShader() const
	{
		return *m_shader;
	}

	void Material::setShader(const Shader& shader)
	{
		m_shader = &shader;
	}

	const Texture* Material::getDiffuse() const
	{
		return m_diffuse;
	}

	void Material::setDiffuse(const Texture* texture)
	{
		m_diffuse = texture;
	}

	void Material::use() const
	{
		m_shader->use();

		if (m_diffuse != nullptr)
			m_diffuse->use();
		else
			Texture::unbind();

		const GLint shininessUniformLoc = m_shader->getUniformLocation("shininess");
		const GLint diffuseUniformLoc = m_shader->getUniformLocation("_texture");

		glUniform1i(diffuseUniformLoc, 0);
		glUniform1f(shininessUniformLoc, m_shininess);
	}
}
