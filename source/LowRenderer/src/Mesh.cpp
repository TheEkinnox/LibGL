#include "Mesh.h"

namespace My::Rendering
{
	Mesh::Mesh(const Model& model)
		: m_model(&model)
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
