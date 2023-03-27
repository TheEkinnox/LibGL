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
						Mesh(const Mesh& other) = default;
						Mesh(Mesh&& other) noexcept = default;
						~Mesh() = default;

		Mesh&			operator=(const Mesh& other) = default;
		Mesh&			operator=(Mesh&& other) noexcept = default;

		void			setModel(const Model& model);

		void			draw() const;

	private:
		const Model*	m_model = nullptr;
	};
}
