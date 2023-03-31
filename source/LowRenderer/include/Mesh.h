#pragma once
#include "Material.h"
#include "Transform.h"
#include "Scene.h"

namespace My::Resources
{
	class Model;
	class Texture;
}

namespace My::Rendering
{
	using namespace Physics;
	using namespace Resources;

	class Mesh : public SceneNode
	{
	public:
						Mesh() = delete;
		explicit		Mesh(SceneNode* parent, const Model& model, const Material& material);
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
		 * \brief Sets the mesh's material
		 * \param material The mesh's new material
		 */
		void setMaterial(const Material& material);

		/**
		 * \brief Draws the mesh
		 */
		void draw() const;

		/**
		 * \brief Updates the mesh
		 */
		void update() override;

	private:
		const Model*	m_model = nullptr;
		Material		m_material;
	};
}
