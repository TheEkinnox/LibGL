#pragma once
#include "Transform.h"
#include "Model.h"
#include "Scene.h"

namespace My::Rendering
{
	using namespace Physics;
	using namespace Resources;

	class Mesh : public SceneNode
	{
	public:
						Mesh() = delete;
		explicit		Mesh(SceneNode* parent, const Model& model);
						Mesh(const Mesh& other) = default;
						Mesh(Mesh&& other) noexcept = default;
						~Mesh() override = default;

		Mesh&			operator=(const Mesh& other) = default;
		Mesh&			operator=(Mesh&& other) noexcept = default;

		/**
		 * \brief Sets the mesh's model
		 * \param model The mesh's new model
		 */
		void setModel(const Model& model);

		/**
		 * \brief Draws the mesh
		 */
		void draw() const;

	private:
		const Model*	m_model = nullptr;
	};
}
