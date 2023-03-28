#include "Mesh.h"

namespace My::Rendering
{
	Mesh::Mesh(SceneNode* parent, const Model& model)
		: SceneNode(parent, Transform()), m_model(&model)
	{
	}

	void Mesh::setModel(const Model& model)
	{
		m_model = &model;
	}

	void Mesh::draw() const
	{
		m_model->draw();
	}

}
