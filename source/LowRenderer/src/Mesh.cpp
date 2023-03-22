#include "Mesh.h"

namespace My::Rendering
{
	Mesh::Mesh(const Model& model)
		: m_model(&model)
	{
	}

	Mesh::Mesh(const Mesh& other)
		: m_model(other.m_model)
	{
	}

	Mesh::Mesh(Mesh&& other) noexcept
		: m_transform(std::move(other.m_transform)), m_model(other.m_model)
	{
		other.m_model = nullptr;
	}

	Mesh& Mesh::operator=(const Mesh& other)
	{
		if (&other == this)
			return *this;

		m_transform = other.m_transform;
		m_model = other.m_model;

		return *this;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_transform = std::move(other.m_transform);
		m_model = other.m_model;

		other.m_model = nullptr;

		return *this;
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
