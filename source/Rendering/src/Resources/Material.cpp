#include "Resources/Material.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

#include <glad/glad.h>

using namespace LibGL::Resources;

namespace LibGL::Rendering
{
	Material::Material(const Shader& shader, const Texture* diffuse,
		const Texture* specular, const float shininess) : m_shader(&shader),
		m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
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

		glActiveTexture(GL_TEXTURE0);
		if (m_diffuse != nullptr)
			m_diffuse->use();
		else
			Texture::unbind();

		glActiveTexture(GL_TEXTURE1);
		if (m_specular != nullptr)
			m_specular->use();
		else
			Texture::unbind();

		const GLint shininessUniformLoc = m_shader->getUniformLocation("_material.shininess");
		const GLint diffuseUniformLoc = m_shader->getUniformLocation("_material.diffuse");
		const GLint specularUniformLoc = m_shader->getUniformLocation("_material.specular");

		glUniform1i(diffuseUniformLoc, 0);
		glUniform1i(specularUniformLoc, 1);
		glUniform1f(shininessUniformLoc, m_shininess);
	}
}
