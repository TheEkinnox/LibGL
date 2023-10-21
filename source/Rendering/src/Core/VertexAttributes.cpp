#include <glad/glad.h>
#include <Vector/Vector3.h>

#include "Core/VertexAttributes.h"

using namespace LibMath;

namespace LibGL::Rendering
{
	VertexAttributes::VertexAttributes(const VertexBuffer& vbo, const IndexBuffer& ebo)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		vbo.bind();
		ebo.bind();

		constexpr auto stride = sizeof(Vertex);

		// position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);

		// normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(sizeof(Vector3)));

		// texture coordinates attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(2 * sizeof(Vector3)));
	}

	VertexAttributes::VertexAttributes(VertexAttributes&& other) noexcept
		: m_vao(other.m_vao)
	{
		other.m_vao = 0;
	}

	VertexAttributes::~VertexAttributes()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	VertexAttributes& VertexAttributes::operator=(
		VertexAttributes&& other) noexcept
	{
		if (&other == this)
			return *this;

		glDeleteVertexArrays(1, &m_vao);

		m_vao = other.m_vao;

		other.m_vao = 0;

		return *this;
	}

	void VertexAttributes::bind() const
	{
		glBindVertexArray(m_vao);
	}

	void VertexAttributes::unbind()
	{
		glBindVertexArray(0);
	}
}