#include "LowRenderer/Mesh.h"

#include <glad/glad.h>

#include "LowRenderer/Camera.h"
#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"

using namespace LibMath;
using namespace LibGL::Resources;

namespace LibGL::Rendering
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
		const Matrix4 viewProjMat = Camera::getCurrent().getViewProjectionMatrix();
		const Matrix4 modelMat = getGlobalTransform().getMatrix();

		shader.setUniformMat4("u_mvp", viewProjMat * modelMat);
		shader.setUniformMat4("u_modelMat", modelMat);
		shader.setUniformMat4("u_normalMat", modelMat.inverse().transposed());

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
