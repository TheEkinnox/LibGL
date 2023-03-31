#include "Mesh.h"

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"

namespace My::Rendering
{
	Mesh::Mesh(SceneNode* parent, const Model& model, const Material& material)
		: SceneNode(parent, Transform()), m_model(&model), m_material(material)
	{
	}

	void Mesh::setModel(const Model& model)
	{
		m_model = &model;
	}

	void Mesh::setMaterial(const Material& material)
	{
		m_material = material;
	}

	void Mesh::draw() const
	{
		m_material.use();

		const Shader& shader = m_material.getShader();

		const GLint mvpMatUniformLoc = shader.getUniformLocation("mvp");
		const GLint modelMatUniformLoc = shader.getUniformLocation("modelMat");
		const GLint normalMatUniformLoc = shader.getUniformLocation("normalMat");

		const Matrix4 viewProjMat = Camera::getCurrent().getViewProjectionMatrix();

		const Matrix4 modelMat = getGlobalTransform().getMatrix();
		const Matrix4 normalMat = modelMat.inverse().transposed();
		const Matrix4 mvpMat = viewProjMat * modelMat;

		glUniformMatrix4fv(mvpMatUniformLoc, 1, GL_TRUE, mvpMat.getArray());
		glUniformMatrix4fv(modelMatUniformLoc, 1, GL_TRUE, modelMat.getArray());
		glUniformMatrix4fv(normalMatUniformLoc, 1, GL_TRUE, normalMat.getArray());

		m_model->draw();

		Shader::unbind();
		Texture::unbind();
	}

	void Mesh::update()
	{
		draw();

		SceneNode::update();
	}
}
