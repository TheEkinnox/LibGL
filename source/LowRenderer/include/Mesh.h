#pragma once
#include "Transform.h"
#include "Model.h"

namespace My::Rendering
{
	using namespace Physics;
	using namespace Resources;

	class Mesh
	{
	public:
		Transform		m_transform;

						Mesh() = delete;
		explicit		Mesh(const Model& model);
						Mesh(const Mesh& other);
						Mesh(Mesh&& other) noexcept;
						~Mesh() = default;

		Mesh&			operator=(const Mesh& other);
		Mesh&			operator=(Mesh&& other) noexcept;

		void			setModel(const Model& model);

		void			draw() const;

	private:
		const Model*	m_model = nullptr;
	};
}
